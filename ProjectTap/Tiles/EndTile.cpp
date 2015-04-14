// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "EndTile.h"

AEndTile::AEndTile(const FObjectInitializer& initializer) : ATile(initializer)
{
	PrimaryActorTick.bCanEverTick = true;
	FName path("/Game/Models/End");
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(1,1,1), false);
	}
}

void AEndTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(BoxCollision->IsOverlappingActor(ball))
	{
		ball->GetRootPrimitiveComponent()->SetSimulatePhysics(false);
	}
}
