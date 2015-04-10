// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "StrongTile.h"


// Sets default values
AStrongTile::AStrongTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStrongTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStrongTile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

