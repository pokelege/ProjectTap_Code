// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameState.h"
#include "Engine/GameInstance.h"
#include "EndTile.h"

const FName AEndTile::END_MESH = FName("/Game/Models/End");

AEndTile::AEndTile() : ATile()
{
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*END_MESH.ToString());
	TileMesh->SetStaticMesh(mesh.Object);

	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(0.1f,0.1f,1), false);
	}

	BoxCollision->bGenerateOverlapEvents = true;
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(40.0f, 40.0f, 80.0f));
	delegate.BindUFunction(this, TEXT("OnBeginTriggerOverlap"));
	BoxCollision->OnComponentBeginOverlap.Add(delegate);
}

void AEndTile::OnBeginTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	if (Cast<ABallPawn>(OtherActor) != nullptr)
	{
		auto pc = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		pc->SetSimulatePhysics(false);
		UGameInstance* gameInstance = GetGameInstance();
		FWorldContext* worldContext = gameInstance->GetWorldContext();
		UWorld* world = worldContext->World();
		AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
		if (gameState) gameState->SetState(AProjectTapGameState::GAME_STATE_WIN);

	}

}

void AEndTile::Highlight(bool litTile , bool litEdge)
{

}
