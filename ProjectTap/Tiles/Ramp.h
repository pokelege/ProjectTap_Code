// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "Tiles/PlayerBall.h"
#include "Ramp.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTTAP_API ARamp : public ATile
{
	GENERATED_BODY()
	FMatrix originalMatrix;
	float time;
	float duration;
	bool reverse;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	FVector pivot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UCurveFloat* rotationSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UBoxComponent* BallCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	APlayerBall* ball;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	FVector moveDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	float forceMultiplier;


	ARamp();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;
};
