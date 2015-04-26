// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "DeflectiveTile.h"



ADeflectiveTile::ADeflectiveTile()
{
	PrimaryActorTick.bCanEverTick = true;

	FName path("/Game/Models/DeflectingTile");
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path.ToString());
	TileMesh->SetStaticMesh(mesh.Object);


	BoxCollision->SetBoxExtent(FVector(1.3f, 1.3f, 1.3f));

	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollision->SetWorldScale3D(FVector(2.0f, 40.0f, 80.0f));

	//BoxCollision->SetWorldRotation(FRotator(0, 45, 0));
}


int ADeflectiveTile::GetCurrentRotation()
{
	return currentRotation % 360;
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

	if (activated)
	{
		Spin(DeltaTime);
	}

}

void ADeflectiveTile::Spin(float dt)
{
	static float accum = 0;

	BoxCollision->SetWorldRotation(FRotator(0, accum, 0));

	accum += 0.2f;
}

void ADeflectiveTile::Highlight(bool litTile, bool litEdge)
{

}

void ADeflectiveTile::CancelHighlight()
{
	
}
