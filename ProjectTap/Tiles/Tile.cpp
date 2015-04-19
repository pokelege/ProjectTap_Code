// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Tile.h"
#include "EmptyComponent.h"
// Sets default values
ATile::ATile() 
{
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Tile mesh" ) );

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Tile collision"));

	UEmptyComponent* root = CreateDefaultSubobject<UEmptyComponent>(TEXT("Tile root"));

	this->SetRootComponent(root);
	
	BoxCollision->AttachTo(this->GetRootComponent());
	TileMesh->AttachTo(this->GetRootComponent());

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