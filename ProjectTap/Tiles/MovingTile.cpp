// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MovingTile.h"


// Sets default values
AMovingTile::AMovingTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovingTile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

