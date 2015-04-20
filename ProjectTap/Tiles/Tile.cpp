// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Tile.h"

// Sets default values
ATile::ATile() 
{
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Tile mesh" ) );

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Tile collision"));

	this->SetRootComponent(BoxCollision);
	
	TileMesh->AttachTo(BoxCollision);

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

void ATile::Highlight()
{
	auto m = TileMesh->GetMaterial(0);
	m->SetOverrideEmissiveBoost(true);
	m->SetEmissiveBoost(100.0f);
	
}

void ATile::CancelHighlight()
{
	auto m = TileMesh->GetMaterial(0);
	m->SetEmissiveBoost(0.0f);
}