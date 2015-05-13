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

}

void ADraggableMoveTile::DragTo(const FHitResult& hit, const FVector cameraLocation)
{
	if (isSelected)
	{

	}
	else
	{
		anchorHitPoint = hit.ImpactPoint;
		cameraRayLength = (anchorHitPoint - cameraLocation).Size();
	}
}