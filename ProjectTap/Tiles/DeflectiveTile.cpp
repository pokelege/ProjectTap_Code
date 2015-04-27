// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "DeflectiveTile.h"



ADeflectiveTile::ADeflectiveTile()
{
	PrimaryActorTick.bCanEverTick = true;
	TileMesh->DetachFromParent();

	FName path("/Game/Models/DeflectingTile");
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	RootComponent = nullptr;
	SetRootComponent(TileMesh);
	BoxCollision->DetachFromParent();
	BoxCollision->SetBoxExtent(FVector(0.0f));

	TileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	TileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	TileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	TileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	TileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//BoxCollision->SetWorldRotation(FRotator(0, 45, 0));
	TileMesh->SetWorldScale3D(FVector(5.0f, 40.0f, 80.0f));
}


// Called when the game starts or when spawned
void ADeflectiveTile::BeginPlay()
{
	Super::BeginPlay();
	switch (type)
	{
	case DeflectiveTileType::HORIZONTAL:
		SetActorRotation(FRotator(0.0f, currentRotation, 0.0f));
		break;
	case DeflectiveTileType::VERTICAL_NORMAL_X:
		SetActorRotation(FRotator(currentRotation, 0.0f, rotationDegreeLimit));
		break;
	case DeflectiveTileType::VERTICAL_NORMAL_Y:
		SetActorRotation(FRotator(currentRotation, rotationDegreeLimit, rotationDegreeLimit));
		break;
	default:
		break;
	}
}

// Called every frame
void ADeflectiveTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Spin(DeltaTime);

}

void ADeflectiveTile::Spin(float dt)
{
	static float accum = 0;

	if (activated && accum < rotationDegreeLimit)
	{
		auto rotationPerFrame = rotationDegreeLimit / rotationDuration * dt;

		accum += rotationPerFrame;

		if (accum >= rotationDegreeLimit)
		{
			accum = 0.0f;
			currentRotation += rotationDegreeLimit;
			currentRotation = static_cast<int>(currentRotation) % 360;
			deactivate();
		}

		switch (type)
		{
			case DeflectiveTileType::HORIZONTAL:
				SetActorRotation(FRotator(0.0f, accum + currentRotation, 0.0f));
				break;
			case DeflectiveTileType::VERTICAL_NORMAL_X:
				SetActorRotation(FRotator(accum + currentRotation, 0.0f, rotationDegreeLimit));
				break;
			case DeflectiveTileType::VERTICAL_NORMAL_Y:
				SetActorRotation(FRotator(accum + currentRotation, rotationDegreeLimit, rotationDegreeLimit));
				break;
			default:
			break;
		}

	}

}

void ADeflectiveTile::Highlight(bool litTile, bool litEdge)
{

}

void ADeflectiveTile::CancelHighlight()
{
	
}


void ADeflectiveTile::activate()
{
	Super::activate();
	Disable();
}

void ADeflectiveTile::deactivate()
{
	Super::deactivate();
	Enable();
}
