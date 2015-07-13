// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "DraggableMoveTile.h"
#include "DataStructure/GVertex.h"
#include "DataStructure/Graph.h"
#include "Classes/Particles/ParticleEmitter.h"
//#include <cassert>

ADraggableMoveTile::ADraggableMoveTile()
{
	PrimaryActorTick.bCanEverTick = true;
	arrowMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow mesh"));
	
	SetRootComponent(TileMesh);
	TileMesh->DetachFromParent();
	BoxCollision->AttachChildren.Remove(TileMesh);
	BoxCollision->AttachTo(TileMesh);
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh( TEXT( "/Game/Models/SM_DragTile" ) );
	TileMesh->SetStaticMesh( mesh.Object );
	FName path("/Game/Models/ArrowPlane");
	ConstructorHelpers::FObjectFinder<UStaticMesh> arrowMesh(*path.ToString());
	arrowMeshComponent->SetStaticMesh(arrowMesh.Object);
	arrowMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	arrowMeshComponent->SetWorldScale3D(FVector(30.0f, 33.0f, 1.0f));
	arrowMeshComponent->SetHiddenInGame(true);
	arrowMeshComponent->AttachTo(RootComponent);

	indicatorParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("indicatorParticle"));
	indicatorParticle->AttachTo(RootComponent);
	ConstructorHelpers::FObjectFinder<UParticleSystem> particleAssets(TEXT("/Game/Particles/P_IndicatorParticle"));
	indicatorParticle->SetTemplate(particleAssets.Object);
}

void ADraggableMoveTile::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void ADraggableMoveTile::Initialize()
{
	if (currentVertex != nullptr)
	{
		SetActorLocation(currentVertex->GetActorLocation());
		currentVertex->SetOccupied(true);		
	}

	if (carryOn != nullptr)
	{		
		if (auto actor = Cast<ICarriable>(carryOn))
		{
			auto scale = actor->getOffsetInfo().scaleForCollision;
			auto offset = actor->getOffsetInfo().offsetForCollision;
			BoxCollision->SetRelativeScale3D(scale);
			BoxCollision->AddLocalOffset(offset);
		}
	}
}


void ADraggableMoveTile::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);

	UpdateDragMove(DeltaTime);
	UpdateIndicator();
}

void ADraggableMoveTile::UpdateIndicator()
{
	auto indicatorBody = indicatorParticle->EmitterInstances[0];
	auto Z_offset = FVector(0.0f, 0.0f, 30.0f);
	auto sourcePoint = GetActorLocation() + Z_offset;
	auto targetPoint = newGoalPos;
	targetPoint += Z_offset;

	if (isSelected)
	{
		arrowMeshComponent->SetHiddenInGame(false);
		indicatorBody->SetBeamSourcePoint(sourcePoint, 0);
		indicatorBody->SetBeamTargetPoint(targetPoint, 0);
		arrowMeshComponent->SetWorldLocation(targetPoint);
		auto rot = FRotationMatrix::MakeFromX(targetPoint - sourcePoint);
		arrowMeshComponent->SetWorldRotation(rot.Rotator());
	}
	else
	{
		indicatorBody->SetBeamSourcePoint(GetActorLocation(), 0);
		indicatorBody->SetBeamTargetPoint(GetActorLocation(), 0);
		arrowMeshComponent->SetWorldLocation(GetActorLocation());
		arrowMeshComponent->SetWorldRotation(FRotationMatrix::Identity.Rotator());
		arrowMeshComponent->SetHiddenInGame(true);
	}

	auto indicatorBodyMaterial = indicatorParticle->CreateDynamicMaterialInstance(0);
	indicatorBodyMaterial->SetScalarParameterValue(TEXT("IsDestinationValid"), canSnap && !destinationOccupied ? 1.0f : 0.0f);

	auto arrowMaterial = arrowMeshComponent->CreateDynamicMaterialInstance(0);
	arrowMaterial->SetScalarParameterValue(TEXT("IsDestinationValid"), canSnap && !destinationOccupied ? 1.0f : 0.0f);
}

bool ADraggableMoveTile::cameraRayIntersectWithTilePlane(
									 const FVector& camlocation,
									 const FVector& dir,
									 FVector& hitPoint)

{
	bool hitPlane = false;
	auto planeNormal = GetActorUpVector();
	auto demon = FMath::Abs(FVector::DotProduct(dir, planeNormal));
	
	if (demon > FLT_EPSILON)
	{
		auto t = FVector::DotProduct(camlocation - GetActorLocation(), planeNormal) / demon;
		hitPlane = t >= 0;
		if (hitPlane) 
		{
			hitPoint = camlocation + dir * t;
		}
	}

	return hitPlane;
}

void ADraggableMoveTile::HighlightTile()
{
	Super::HighlightTile();
	auto actor = Cast<ATile>(carryOn);
	if (actor != nullptr && !isSelected)
	{
		actor->HighlightTile();
	}
}

void ADraggableMoveTile::HighlightEdge()
{
	Super::HighlightEdge();
	auto actor = Cast<ATile>(carryOn);
	if (actor != nullptr && !isSelected)
	{
		actor->HighlightEdge();
	}
}

void ADraggableMoveTile::CancelHighlightTile()
{
	Super::CancelHighlightTile();
	auto actor = Cast<ATile>(carryOn);
	if (actor != nullptr && !isSelected)
	{
		actor->CancelHighlightTile();
	}
}

void ADraggableMoveTile::CancelHighlightEdge()
{
	Super::CancelHighlightEdge();
	auto actor = Cast<ATile>(carryOn);
	if (actor != nullptr && !isSelected)
	{
		actor->CancelHighlightEdge();
	}
}

void ADraggableMoveTile::processMouseEvents()
{
	auto dt = GetWorld()->GetDeltaSeconds();	
	mousePressTimer += dt;
	
	if (mousePressTimer >= 0.2f)
	{
		isSelected = true;
		mousePressTimer = 0.0f;
	}
	else
	{
		anchorHitPoint = GetActorLocation();
		newGoalPos = GetActorLocation();
		canSnap = false;
		isMoving = false;
	}

	deactivate();
}

void ADraggableMoveTile::DragTo(const FHitResult& hit, 
								const FVector& cameraLocation,
								const FVector& camRayDirection)
{	
	if (isMoving) return;
	if (isSelected && currentVertex != nullptr)
	{
		//use hit point as a camera ray
		auto camRay = FVector();

		auto bCamRayHit = cameraRayIntersectWithTilePlane(cameraLocation, camRayDirection, camRay);		

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
				goalVertex = nullptr;
			}
			
			if (canSnap)
			{
				newGoalPos = mostPossibleVertex->GetActorLocation();
				goalVertex = mostPossibleVertex;
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
		processMouseEvents();
	}
}


void ADraggableMoveTile::UpdateDragMove(float dt)
{
	if (isMoving)
	{		
		auto moveDir = (newGoalPos - GetActorLocation()).GetSafeNormal();
		auto reachedPos = FVector::DistSquared(newGoalPos, GetActorLocation()) < 10.0f;
	
		if (reachedPos)
		{
			SetActorLocation(newGoalPos);
			isMoving = false;
		}
		else if (moveDir.SizeSquared() > 0.1f)
		{
			SetActorLocation(GetActorLocation() + moveDir * dragMoveSpeed * dt);
		}
	}
}

FVector ADraggableMoveTile::calculateCurrentDir()
{
	auto startNode = path[currNode];
	return FVector();
}

void ADraggableMoveTile::click()
{
	if (auto carry = Cast<ATile>(carryOn))
	{
		carry->activate();
	}
	deactivate();
	mousePressTimer = 0.0f;
}

void ADraggableMoveTile::RemoveFocus()
{
	//if this mouse release results in a click event
	if (!isSelected) 
	{
		click();
	}

	isSelected = false;

	bool canMove = canSnap && !destinationOccupied && goalVertex != nullptr;
	if (canMove)
	{
		isMoving = true;
		currentVertex->SetOccupied(false);
		goalVertex->SetOccupied(true);
		currentVertex = goalVertex;
		goalVertex = nullptr;
	}

}
