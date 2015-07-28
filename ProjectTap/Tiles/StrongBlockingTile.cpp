// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "StrongBlockingTile.h"


// Sets default values
AStrongBlockingTile::AStrongBlockingTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FName path("/Game/Models/SM_StrongBlockingTile");
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	move_speed = 400.0f;
	move_distance_tolerance = 150.0f;
}

// Called when the game starts or when spawned
void AStrongBlockingTile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStrongBlockingTile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

