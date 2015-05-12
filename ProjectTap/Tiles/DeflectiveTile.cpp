// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "DeflectiveTile.h"



ADeflectiveTile::ADeflectiveTile()
{
	PrimaryActorTick.bCanEverTick = true;

	FName path("/Game/Models/DeflectingTile");
	FName framePath("/Game/Models/DeflectingTileFrame");
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path.ToString());
	ConstructorHelpers::FObjectFinder<UStaticMesh> frameMesh(*framePath.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	BoxCollision->SetBoxExtent(FVector(0.9f));
	frameMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Frame mesh" ) );

	frameMeshComponent->SetStaticMesh(frameMesh.Object);
	frameMeshComponent->AttachTo(TileMesh);

	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	TileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//BoxCollision->SetWorldRotation(FRotator(0, 45, 0));
	BoxCollision->SetWorldScale3D(FVector(5.0f, 40.0f, 80.0f));

	glowPowerHighlighted = 70.0f;
	glowColor = FLinearColor(0.03f, 0.07f, 1.0f);
	baseColor = FLinearColor(0.72f, 3.0f, 0.74, 0.5f);

	glowColorHighlighted *= glowColor;
	baseColorHighlighted *= glowColor;
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
	Super::Highlight();
}

void ADeflectiveTile::CancelHighlight()
{
	Super::CancelHighlight();
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
	Super::CancelHighlight();
}
