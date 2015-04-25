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
		BoxCollision->SetBoxExtent(FVector(1,1,1), false);
		BoxCollision->SetRelativeLocation(FVector(0,0,-1), false, nullptr);
	}
	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(40.0f, 40.0f, 80.0f));
	delegate.BindUFunction(this, "OnHit");
	BoxCollision->OnComponentHit.Add(delegate);
}

void AEndTile::OnHit(AActor* OtherActor,
		   UPrimitiveComponent* OtherComp,
		   FVector NormalImpulse,
		   const FHitResult& Hit)
{
	if(Cast<ABallPawn>(OtherActor) != nullptr)
	{
		auto pc = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		pc->SetSimulatePhysics(false);
		UGameInstance* gameInstance = GetGameInstance();
		FWorldContext* worldContext = gameInstance->GetWorldContext();
		UWorld* world = worldContext->World();
		AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
		if(gameState) gameState->SetState(AProjectTapGameState::GAME_STATE_WIN);
	}
}

void AEndTile::Highlight(bool litTile , bool litEdge)
{

}
