// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "DraggableMoveTile.h"
#include "../DataStructure/GVertex.h"
#include "../DataStructure/Graph.h"
#include "Classes/Particles/ParticleEmitter.h"
#include <cassert>

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
	assert(currentVertex != nullptr);
	SetActorLocation(currentVertex->GetActorLocation());
	currentVertex->SetOccupied(true);
}

void ADraggableMoveTile::Tick( float DeltaTime )
{
	UpdateDragMove(DeltaTime);
	UpdateIndicator();
}
#define printonscreen(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

void ADraggableMoveTile::UpdateIndicator()
{
	auto indicatorBody = indicatorParticle->EmitterInstances[0];
	auto Z_offset = FVector(0.0f, 0.0f, 30.0f);
	auto sourcePoint = GetActorLocation() + Z_offset;
	auto targetPoint = newGoalPos;
	targetPoint.Z = sourcePoint.Z;

	if (isSelected)
	{
		indicatorBody->SetBeamSourcePoint(sourcePoint, 0);
		indicatorBody->SetBeamTargetPoint(targetPoint, 0);
		printonscreen(targetPoint.ToString());
	}
	else
	{
		indicatorBody->SetBeamSourcePoint(GetActorLocation(), 0);
		indicatorBody->SetBeamTargetPoint(GetActorLocation(), 0);
	}

	auto material = indicatorParticle->CreateDynamicMaterialInstance(0);
	material->SetScalarParameterValue(TEXT("IsDestinationValid"), canSnap && !destinationOccupied ? 1.0f : 0.0f);
}

bool ADraggableMoveTile::cameraRayIntersectWithTilePlane(const FVector& camlocation,
									 const FVector& dir,
									 FVector& hitPoint)

{
	bool hitPlane = false;
	//auto demo = FVector::DotProduct(camlocation, dir) + GetActorLocation();

	return hitPlane;
}

void ADraggableMoveTile::DragTo(const FHitResult& hit, 
								const FVector& cameraLocation,
								const FVector& camRayDirection)
{
	if (isSelected)
	{
		//use hit point as a camera ray
		auto camRay = FVector();
		if (camRay.Equals(FVector::ZeroVector))
		{
			//FVector::ray
			cameraRayLength = FVector::Dist(cameraLocation, GetActorLocation());
			camRay = cameraLocation + camRayDirection * cameraRayLength;
		}

		auto moveRay = camRay - anchorHitPoint;
		auto deltaLength = moveRay.SizeSquared();

		if (deltaLength > dragTolerance * dragTolerance)
		{
			auto mostPossibleVertex = currentVertex->getGraph()->FindNearestVertexTo(moveRay, currentVertex, dragTolerance * dragTolerance);

			if (mostPossibleVertex != nullptr)
			{
				auto nextDir = mostPossibleVertex->GetActorLocation() - GetActorLocation();
				auto moveDelta = moveRay.ProjectOnTo(nextDir);
				auto isMoveRayAndNextDirSameDirection = FVector::DotProduct(moveDelta, nextDir) > 0.0f;

				if (isMoveRayAndNextDirSameDirection && moveDelta.SizeSquared() > dragTolerance * dragTolerance)
				{
					newGoalPos = moveDelta + anchorHitPoint;
					canSnap = true;
					destinationOccupied = mostPossibleVertex->IsVertexOccupied();
				}
			}

			if (mostPossibleVertex == nullptr)
			{
				canSnap = false;
			}
			
			if (canSnap)
			{
				newGoalPos = mostPossibleVertex->GetActorLocation();
			}
			else 
			{
				newGoalPos = camRay;
			}
		}
		else
		{
			canSnap = false;
			newGoalPos = GetActorLocation() + moveRay;
		}
	}
	else
	{
		anchorHitPoint = GetActorLocation();		
		newGoalPos = GetActorLocation();
		isSelected = true;
		canSnap = false;
		isMoving = false;
	}
}

void ADraggableMoveTile::UpdateDragMove(float dt)
{
	if (isMoving)
	{		
		auto moveDir = (newGoalPos - GetActorLocation()).GetSafeNormal();
		auto reachedPos = FVector::DistSquared(newGoalPos, GetActorLocation()) < 1.0f;
	
		if (reachedPos)
		{
			//SetActorLocation(newGoalPos);
			isMoving = false;
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
	isMoving = true;
}
