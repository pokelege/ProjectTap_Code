// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MovingTile.h"
#include "DraggableMoveTile.generated.h"

UCLASS()
class PROJECTTAP_API ADraggableMoveTile : public AMovingTile
{
	GENERATED_BODY()

	FVector anchorHitPoint;
	float cameraRayLength;
	bool isSelected = false;
		
public:	
	// Sets default values for this actor's properties
	ADraggableMoveTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void DragTo(const FHitResult& hit, const FVector cameraLocation);
	
};
