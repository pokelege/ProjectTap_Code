// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "Pawns/BallPawn.h"
#include "Pawns/PawnCastingTrigger.h"
#include "Tiles/BaseRampTile.h"
#include "Ramp.generated.h"

UENUM(BlueprintType)
enum class Direction : uint8
{
	XPlus,
	YPlus,
	xMinus,
	yMinus
};

/**
 * 
 */
UCLASS()
class PROJECTTAP_API ARamp : public ABaseRampTile
{
	GENERATED_BODY()

	static const FName RAMP_MESH_PATH;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	Direction rotationDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	float forceMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		float additionalZForce = 3000.0f;

	ARamp();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

	virtual void activate() override;

};
