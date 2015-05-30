// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "DraggableMoveTile.h"
#include "../DataStructure/GVertex.h"
#include "Classes/Particles/ParticleEmitter.h"

ADraggableMoveTile::ADraggableMoveTile()
{
	PrimaryActorTick.bCanEverTick = true;
	arrowMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow mesh"));

	FName path("/Game/Models/ArrowPlane");
	ConstructorHelpers::FObjectFinder<UStaticMesh> arrowMesh(*path.ToString());
	arrowMeshComponent->SetStaticMesh(arrowMesh.Object);
	arrowMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	arrowMeshComponent->AttachTo(RootComponent);

	indicatorParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("indicatorParticle"));
	indicatorParticle->AttachTo(RootComponent);
	ConstructorHelpers::FObjectFinder<UParticleSystem> particleAssets(TEXT("/Game/Particles/P_IndicatorParticle"));
	indicatorParticle->SetTemplate(particleAssets.Object);
}

void ADraggableMoveTile::BeginPlay()
{
	Super::BeginPlay();
}

void ADraggableMoveTile::Tick( float DeltaTime )
{
	UpdateDragMove(DeltaTime);
}

void ADraggableMoveTile::UpdateIndicator()
{
	auto indicatorBody = indicatorParticle->EmitterInstances[0];
	indicatorBody->SetBeamSourcePoint(GetActorLocation(), 0);
	indicatorBody->SetBeamTargetPoint(FVector(), 0);
}


void ADraggableMoveTile::DragTo(const FHitResult& hit, 
								const FVector& cameraLocation,
								const FVector& camRayDirection)
{
	if (isSelected)
	{
		auto camRay = cameraLocation + camRayDirection * cameraRayLength;
		auto moveRay = camRay - anchorHitPoint;

		auto deltaLength = moveRay.SizeSquared();
		if (deltaLength > dragTolerance * dragTolerance)
		{
			auto moveDelta = moveRay.ProjectOnTo(FVector(0.0f, 1.0f, 0.0f));

			if (moveDelta.Size() > dragTolerance * dragTolerance)
			{
				newGoalPos = moveDelta + anchorHitPoint;
			}
			else
			{
				isSelected = false;
				newGoalPos = GetActorLocation();
			}
		}
		else
		{
			isSelected = false;
			newGoalPos = GetActorLocation();
		}
	}
	else
	{
		anchorHitPoint = GetActorLocation();
		cameraRayLength = (anchorHitPoint - cameraLocation).Size();
		newGoalPos = GetActorLocation();
		isSelected = true;
	}
}

void ADraggableMoveTile::UpdateDragMove(float dt)
{
	if (isSelected)
	{		
		auto moveDir = (newGoalPos - GetActorLocation()).GetSafeNormal();
		auto reachedPos = FVector::DistSquared(newGoalPos, GetActorLocation()) < 1.0f;
	
		if (reachedPos)
		{
			//SetActorLocation(newGoalPos);
			UpdateIndicator();
		}
		else if (moveDir.SizeSquared() > 0.1f)
		{
			//SetActorLocation(GetActorLocation() + moveDir * dragMoveSpeed * dt);
		}
	}
}

FVector ADraggableMoveTile::calculateCurrentDir()
{
	auto startNode = path[currNode];
	return FVector();
}


void ADraggableMoveTile::RemoveFocus()
{
	isSelected = false;
}
