// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "DeflectiveTile.h"
#include "../Pawns/BallPawn.h"


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
	frameMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	frameMeshComponent->AttachTo(TileMesh);

	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	TileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	BoxCollision->OnComponentHit.AddDynamic(this, &ADeflectiveTile::OnHit);

	//BoxCollision->SetWorldRotation(FRotator(0, 45, 0));
	BoxCollision->SetWorldScale3D(FVector(5.0f, 40.0f, 80.0f));

	glowPowerHighlighted = 70.0f;

}


// Called when the game starts or when spawned
void ADeflectiveTile::BeginPlay()
{
	Super::BeginPlay();

	glowColor = FLinearColor(0.3f, 0.7f, 1.0f);
	baseColor = FLinearColor(0.72f, 3.0f, 0.74, 0.5f);

	glowColorHighlighted *= glowColor;
	baseColorHighlighted *= baseColor;
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

	UpdateEdgeHighlight(DeltaTime);

}

void ADeflectiveTile::UpdateEdgeHighlight(float dt)
{
	static bool glowPowerIncreased = false;

	if (edgeHighlightDuration > 0.0f)
	{
		if (!glowPowerIncreased)
		{
			glowColorHighlighted *= glowPower;
			glowPowerIncreased = true;
		}

		HighlightEdge();
		edgeHighlightTimer += dt;

		edgeHighlightDuration = edgeHighlightTimer >= edgeHighlightDuration ? 0.0f : edgeHighlightDuration;

		if (edgeHighlightDuration == 0.0f)
		{
			edgeHighlightTimer = 0.0f;
			glowColorHighlighted /= glowPower;
			glowPowerIncreased = false;
			CancelHighlightEdge();
		}
	}
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

		ballCanTouch = false;
	}
	else
	{
		ballCanTouch = true;
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

void ADeflectiveTile::HighlightEdgeForDuration(float duration)
{
	edgeHighlightDuration = duration;
}


FVector ClampNormalAxis(FVector dir)
{
	int x = dir.X;
	int y = dir.Y;
	FVector normal;

	if (FMath::Abs(x) > FMath::Abs(y))
	{
		normal.X = x > 0.0f ? 1.0f : -1.0f;
		normal.Y = 0.0f;
		normal.Z = 0.0f;
	}
	else
	{
		normal.Y = x > 0.0f ? 1.0f : -1.0f;
		normal.X = 0.0f;
		normal.Z = 0.0f;
	}

	return normal;
}

void ADeflectiveTile::OnHit(class AActor* OtherActor, 
							class UPrimitiveComponent* OtherComp,
							FVector NormalImpulse, 
							const FHitResult& Hit)
{

	if (auto ball = Cast<ABallPawn>(OtherActor))
	{
		if (ballCanTouch)
		{
			auto incomingVector = GetActorLocation() - ball->GetActorLocation();
			incomingVector.Z = 0.0f;
			auto newDir = FMath::GetReflectionVector(incomingVector, NormalImpulse);
			auto newVel = 500 * ClampNormalAxis(newDir.SafeNormal());
			ball->ballCollision->SetPhysicsAngularVelocity(FVector::ZeroVector);
			ball->ballCollision->SetPhysicsLinearVelocity(newVel);
			ball->ResetBallXYPosition(GetActorLocation());
			HighlightEdgeForDuration(0.3f);
		}
		else
		{
			ball->Kill();
		}
	}

}
