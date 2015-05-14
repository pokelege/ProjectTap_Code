// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "DraggableMoveTile.h"


ADraggableMoveTile::ADraggableMoveTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void ADraggableMoveTile::BeginPlay()
{
	Super::BeginPlay();
}

void ADraggableMoveTile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	UpdateDragMove(DeltaTime);
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
			newGoalPos = moveDelta + anchorHitPoint;

			if (moveDelta.Size() > 300)
			{
				int i = 0;
			}
		}
	}
	else
	{
		anchorHitPoint = GetActorLocation();
		cameraRayLength = (anchorHitPoint - cameraLocation).Size();
		isSelected = true;
	}
}

void ADraggableMoveTile::UpdateDragMove(float dt)
{
	if (isSelected)
	{		
		auto moveDir = newGoalPos - GetActorLocation();
		auto reachedPos = FVector::DistSquared(newGoalPos, GetActorLocation()) < 1.0f;
		if (reachedPos)
		{
			SetActorLocation( newGoalPos);
		}
		else
		{
			SetActorLocation(GetActorLocation() + FVector(0.0f, 1.0f, 0.0f) * dragMoveSpeed * dt);
		}
	}
}


void ADraggableMoveTile::RemoveFocus()
{
	isSelected = false;
}
