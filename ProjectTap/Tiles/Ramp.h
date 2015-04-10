// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "Runtime/SlateCore/Public/Animation/CurveSequence.h"
#include "Ramp.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTTAP_API ARamp : public ATile
{
	GENERATED_BODY()
	FMatrix originalMatrix;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	FVector pivot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	FCurveSequence* rotationSequence;
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;
	virtual void activate() override;
};
