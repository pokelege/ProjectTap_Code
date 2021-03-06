// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "CustomGameState.h"
#include "ICarriable.h"
#include "BaseRampTile.generated.h"

UENUM( BlueprintType )
enum class Direction : uint8
{
	Guess ,
	XPlus ,
	YPlus ,
	xMinus ,
	yMinus
};

/**
 *
 */
UCLASS()
class PROJECTTAP_API ABaseRampTile : public ATile, public ICarriable
{
	GENERATED_BODY()

	friend class APawnCastingTrigger;
	static const FName BASE_RAMP_CURVE_PATH;
	static const GroundableInfo groundableInfo;
protected:
	FDelegateHandle OnGameStateChangedDelegateHandle;
	class ABallPawn* ball = nullptr;
	class ABallPawn* lastPauseBall = nullptr;
	USceneComponent* offset =nullptr;

	void ResetMeshOrientation();
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		Direction rotationDirection = Direction::XPlus;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		UCurveFloat* rotationSequence = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
		UAudioComponent* flipSound = nullptr;
protected:
	float time = 0;
public:
	ABaseRampTile();
	OffsetInfo getOffsetInfo() override;

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;
	virtual const struct GroundableInfo* GetGroundableInfo() const override;
	virtual void activate() override;
	virtual void HighlightEdge() override;
	virtual void CancelHighlightEdge() override;
	virtual void HighlightTile() override;
	virtual void CancelHighlightTile() override;
	UFUNCTION()
		virtual void OnGameStateChanged( const CustomGameState gameState ) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;
#endif
};
