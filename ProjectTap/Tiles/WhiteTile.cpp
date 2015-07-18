// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "WhiteTile.h"


// Sets default values
AWhiteTile::AWhiteTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*FName("/Game/Models/GroundTiles/1x1").ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	TileMesh->AttachTo(BoxCollision);
	TileMesh->SetMobility( EComponentMobility::Static );
	BoxCollision->SetBoxExtent(FVector(40.0f, 40.0f, 80.0f));
}

// Called when the game starts or when spawned
void AWhiteTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWhiteTile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

OffsetInfo AWhiteTile::getOffsetInfo()
{
	OffsetInfo data;
	data.offsetForCollision = FVector(0.0f, 0.0f, 40.0f);
	data.scaleForCollision = FVector(1.2f, 1.2f, 5.0f);
	data.offsetForCarryOn = FVector(0.0f, 0.0f, 85.0f);
	return data;
}


