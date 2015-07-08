// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "Pawns/BallPawn.h"
#include "BaseRampTile.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API ABaseRampTile : public ATile
{
	GENERATED_BODY()

	static const FName BASE_RAMP_CURVE_PATH;

protected:
	FScriptDelegate pawnIn;
	FScriptDelegate pawnOut;
	float time = 0;

	friend class APawnCastingTrigger;
	ABallPawn* ball = nullptr;
	USceneComponent* offset;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UCurveFloat* rotationSequence;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
	UAudioComponent* flipSound = nullptr;

	ABaseRampTile();

	virtual void Tick( float DeltaTime ) override;

	virtual void activate() override;
	virtual void HighlightEdge() override;
	virtual void CancelHighlightEdge() override;
	virtual void HighlightTile() override;
	virtual void CancelHighlightTile() override;
};
