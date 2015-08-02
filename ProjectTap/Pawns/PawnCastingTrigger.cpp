// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "PawnCastingTrigger.h"
#include "BallPawn.h"
#include "Tiles/BlockingTileBase.h"
#include "Tiles/BaseRampTile.h"
#include "ProjectTapGameState.h"
#include "Engine/GameInstance.h"
#include "../Tiles/EndTile.h"

// Sets default values
APawnCastingTrigger::APawnCastingTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	tileOverlapCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TileOverlap"));
	SetRootComponent(tileOverlapCollision);
	tileOverlapCollision->bGenerateOverlapEvents = true;

	tileOverlapCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	tileOverlapCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	tileOverlapCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	tileOverlapCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	tileOverlapCollision->SetBoxExtent(FVector(1.0f));

	SetActorScale3D(FVector(20.0f, 20.0f, 15.0f));

	FScriptDelegate beginOverLap;
	beginOverLap.BindUFunction(this, "OnBeginTriggerOverlap");
	tileOverlapCollision->OnComponentBeginOverlap.Add(beginOverLap);

	FScriptDelegate endEndLap;
	endEndLap.BindUFunction(this, "OnEndTriggerOverlap");
	tileOverlapCollision->OnComponentEndOverlap.Add(endEndLap);
}

void APawnCastingTrigger::BeginPlay()
{
	Super::BeginPlay();
	UWorld* world = GetWorld();
	AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
	OnGameStateChangedDelegateHandle = gameState->GameStateChanged.AddUFunction( this , TEXT( "OnStateChanged" ) );
}

void APawnCastingTrigger::SetBallPawn(ABallPawn* pawn)
{
	this->currentPawn = pawn;
}

void APawnCastingTrigger::OnBeginTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	if ( !canTrigger )
	{
		OnEndTriggerOverlap( OtherActor , OtherComp , OtherBodyIndex , bFromSweep , SweepResult );
		return;
	}
	isOverlaping = true;

	if (castType == BallCastType::CAST_BLOCKING)
	{
		auto tile = Cast<ABlockingTileBase>(OtherActor);
		if (tile != nullptr && !tile->isActivated())
		{
			tile->deactivate();
			tile->Disable();
			tile->turnOffHighlight();
		}
	}
	else if (castType == BallCastType::CAST_RAMP)
	{
		auto ramp = Cast<ABaseRampTile>(OtherActor);
		if (ramp != nullptr)
		{
			ramp->ball = currentPawn;
			ramp->Enable();
			ramp->HighlightTile();
		}

		if (ramp == nullptr)
		{
			auto endTile = Cast<AEndTile>(OtherActor);
			if (endTile != nullptr)
			{
				endTile->EndLevel(currentPawn);
			}
		}
	}

}

void APawnCastingTrigger::OnEndTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	
	if (castType == BallCastType::CAST_BLOCKING)
	{
		auto tile = Cast<ABlockingTileBase>(OtherActor);
		if (tile != nullptr && !tile->isActivated())
		{
			tile->Enable();
			tile->CancelHighlight();
		}
	}

	if (castType == BallCastType::CAST_RAMP)
	{
		auto ramp = Cast<ABaseRampTile>(OtherActor);
		if (ramp != nullptr)
		{
			ramp->CancelHighlight();
			ramp->Disable();
			ramp->ball = nullptr;			
		}
	}

	isOverlaping = false;
}
void APawnCastingTrigger::OnStateChanged( const CustomGameState newState )
{
	canTrigger = newState == CustomGameState::GAME_STATE_PLAYING;
}