// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "PawnCastingTrigger.h"
#include "BallPawn.h"
#include "../Tiles/BlockingTileBase.h"
#include "../Tiles/Ramp.h"


// Sets default values
APawnCastingTrigger::APawnCastingTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	tileOverlapCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TileOverlap"));
	SetRootComponent(tileOverlapCollision);
	tileOverlapCollision->bGenerateOverlapEvents = true;

	//tileOverlapCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	tileOverlapCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	tileOverlapCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	tileOverlapCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	tileOverlapCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	tileOverlapCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	tileOverlapCollision->SetBoxExtent(FVector(1.0f));

	SetActorScale3D(FVector(10.0f, 10.0f, 15.0f));

	FScriptDelegate beginOverLap;
	beginOverLap.BindUFunction(this, "OnBeginTriggerOverlap");
	tileOverlapCollision->OnComponentBeginOverlap.Add(beginOverLap);

	FScriptDelegate beginEndLap;
	beginEndLap.BindUFunction(this, "OnEndTriggerOverlap");
	tileOverlapCollision->OnComponentEndOverlap.Add(beginEndLap);


}

void APawnCastingTrigger::SetBallPawn(ABallPawn* pawn)
{
	this->pawn = pawn;
}


// Called when the game starts or when spawned
void APawnCastingTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnCastingTrigger::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (pawn != nullptr)
	{
		
	}

}

void APawnCastingTrigger::OnBeginTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	bool typeFound = false;
	auto tile = Cast<ABlockingTileBase>(OtherActor);
	if (tile != nullptr)
	{
		tile->deactivate();
		tile->Disable();
		tile->turnOffHighlight();
		typeFound = true;
	}

	if (!typeFound)
	{
		auto ramp = Cast<ABaseRampTile>(OtherActor);
		if (ramp != nullptr)
		{
			ramp->Enable();
			ramp->HighlightTile();
			ramp->ball = pawn;
			typeFound = true;
		}
	}

}

void APawnCastingTrigger::OnEndTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	bool typeFound = false;
	auto tile = Cast<ABlockingTileBase>(OtherActor);
	if (tile != nullptr)
	{
		tile->Enable();
		tile->CancelHighlight();
		typeFound = true;
	}

	if (!typeFound)
	{
		auto ramp = Cast<ABaseRampTile>(OtherActor);
		if (ramp != nullptr)
		{
			ramp->CancelHighlight();
			ramp->Disable();
			ramp->ball = nullptr;
			typeFound = true;
		}

	}
}