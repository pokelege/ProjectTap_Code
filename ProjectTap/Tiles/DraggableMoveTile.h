// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MovingTile.h"
#include "DraggableMoveTile.generated.h"

UCLASS()
class PROJECTTAP_API ADraggableMoveTile : public AMovingTile
{
	GENERATED_BODY()

	FVector anchorHitPoint;
	FVector newGoalPos;
	class AGVertex* goalVertex = nullptr;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Indicator )
		UParticleSystemComponent* indicatorParticle = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Indicator )
		UStaticMeshComponent* arrowMeshComponent = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
	class AGVertex* currentVertex = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		float dragTolerance = 300.0f;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		float dragMoveSpeed = 150.0f;
private:
	float cameraRayLength = 0.0f;
	float mousePressTimer = 0.0f;
	bool isMoving = false;
	bool canSnap = false;
	bool destinationOccupied = false;
	bool reachGoalNextFrame = false;
	bool isSelected = false;
public:

	// Sets default values for this actor's properties
	ADraggableMoveTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void HighlightTile() override;
	virtual void HighlightEdge() override;
	virtual void CancelHighlightTile() override;
	virtual void CancelHighlightEdge() override;

	//give a new camera ray projected from mouse position for new goal position
	void DragTo(const FHitResult& hit,
				const FVector& cameraLocation,
				const FVector& camRayDirection);

	void RemoveFocus();

private:
	void Initialize();

	void UpdateDragMove( float dt );

	FVector calculateCurrentDir();

	void UpdateIndicator();

	bool cameraRayIntersectWithTilePlane( const FVector& camlocation ,
										  const FVector& dir ,
										  FVector& hitPoint );

	void processMouseEvents();

	void click();
};
