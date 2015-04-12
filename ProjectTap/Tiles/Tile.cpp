// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Tile.h"

// Sets default values
ATile::ATile(const FObjectInitializer& initializer) :Super(initializer)
{
	TileMesh = initializer.CreateDefaultSubobject<UStaticMeshComponent>( this , TEXT( "Tile mesh" ) );
	this->SetRootComponent( TileMesh );

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));

	BoxCollision->AttachTo(RootComponent);
}

void ATile::activate()
{
	activated = true;
}

void ATile::deactivate()
{
	activated = false;
}

bool ATile::isActivated()
{
	return activated;
}


void ATile::BeginPlay()
{
  Super::BeginPlay();
  original = GetActorLocation();
}

void ATile::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}