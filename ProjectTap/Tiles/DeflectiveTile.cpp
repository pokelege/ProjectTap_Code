// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "DeflectiveTile.h"



ADeflectiveTile::ADeflectiveTile()
{
	PrimaryActorTick.bCanEverTick = true;

	FName path("/Game/Models/DeflectingTile");
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path.ToString());
	TileMesh->SetStaticMesh(mesh.Object);


	BoxCollision->SetBoxExtent(FVector(1.0f, 1.0f, 1.0f));

	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollision->SetWorldScale3D(FVector(2.0f, 40.0f, 80.0f));

	SetActorRotation(FRotator(0.0f, -45.0f, 0.0f));
	
}



// Called when the game starts or when spawned
void ADeflectiveTile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADeflectiveTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	static float degree = 45.0f;

	if (activated && degree < 90.0f)
	{
		degree += 1.0f;
	}

	if (degree >= 90)
	{
		deactivate();
		degree = 0.0;
	}

	SetActorRotation(FRotator(0.0f, degree, 0.0f));

}

