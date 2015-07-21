// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "ICarriable.h"
#include "MovingTile.generated.h"

UCLASS()
class PROJECTTAP_API AMovingTile : public ATile
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Path )
		TArray<FVector> path;
protected:
	FVector currDir;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Path )
		AActor* carryOn = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Path )
		UCurveFloat* moveCurve = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Path )
		int32 currentEditorPathIndex = 0;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Path )
		float speed = 50.0f;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Path )
		float pauseBetweenNodes = 2.0f;
private:
	float pauseTimeCounter = 0.0f;
	float beta = 0.0f;

protected:
	int32 currNode = 0;
	bool pathReversed = false;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	bool enabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	bool reverseRouteWhenDone = true;

#if WITH_EDITOR
	virtual void EditorKeyPressed(FKey Key,
				                  EInputEvent Event) override;

	void AddCurrentLocation();

	void UpdateCurrentLocation();

	void DeleteCurrentLocation();

	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;

#endif 
	// Sets default values for this actor's properties
	AMovingTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void UpdateCarryOn(float dt);
protected:
	void reset();
	void UpdateMovement( float dt );

	int32 NextIndex();

	FVector NextDirection();

	int32 IncrementIndex();
};
