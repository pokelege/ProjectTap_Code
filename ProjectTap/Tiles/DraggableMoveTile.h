// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MovingTile.h"
#include "DraggableMoveTile.generated.h"

UCLASS()
class PROJECTTAP_API ADraggableMoveTile : public AMovingTile
{
	GENERATED_BODY()

	static TArray<class GVertex*> routeGraph;

	FVector anchorHitPoint;
	FVector newGoalPos;
	float cameraRayLength;
		
	void UpdateDragMove(float dt);

	FVector calculateCurrentDir();

	void UpdateIndicator();
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Indicator)
	UParticleSystemComponent* indicatorParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Indicator)
	UStaticMeshComponent* arrowMeshComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	class AGVertex* currentVertex = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	float dragTolerance = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	float dragMoveSpeed = 5.0f;

	// Sets default values for this actor's properties
	ADraggableMoveTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//if camera ray hits this actor then give a new camera ray projected from mouse position for new position
	void DragTo(const FHitResult& hit, 
				const FVector& cameraLocation, 
				const FVector& camRayDirection);



	void RemoveFocus();
	
private:
	bool isSelected = false;
};
