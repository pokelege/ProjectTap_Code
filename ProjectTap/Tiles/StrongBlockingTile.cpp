// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "StrongBlockingTile.h"


// Sets default values
AStrongBlockingTile::AStrongBlockingTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FName path("/Game/Models/StrongBlockingTile");
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path.ToString());

	BoxCollision->SetWorldScale3D(FVector(80.0f, 80.0f, 80.0f));

	move_speed = 400.0f;
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

