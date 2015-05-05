// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "MovingTile.generated.h"

UCLASS()
class PROJECTTAP_API AMovingTile : public ATile
{
	GENERATED_BODY()

	unsigned int currNode = 0;

	float pauseTimeCounter = 0.0f;

	float beta = 0.0f;

	FVector currDir;

	bool pathReversed = false;

	void UpdateMovement(float dt);

	unsigned NextIndex();

	FVector NextDirection();

	unsigned IncrementIndex();

	void reset();
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	TArray<FVector> path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	AActor* carryOn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	float speed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	float pauseBetweenNodes = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	bool enabled = true;

	// Sets default values for this actor's properties
	AMovingTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
