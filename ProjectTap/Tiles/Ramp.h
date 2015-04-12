// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
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
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;
};