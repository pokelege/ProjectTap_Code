// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "CustomGameState.h"
#include "BaseRampTile.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API ABaseRampTile : public ATile
{
	GENERATED_BODY()
	
	friend class APawnCastingTrigger;
	static const FName BASE_RAMP_CURVE_PATH;

protected:
	FDelegateHandle OnGameStateChangedDelegateHandle;
	class ABallPawn* ball = nullptr;
	class ABallPawn* lastPauseBall = nullptr;
	USceneComponent* offset =nullptr;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		UCurveFloat* rotationSequence = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
		UAudioComponent* flipSound = nullptr;
protected:
	float time = 0;
public:
	ABaseRampTile();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

	virtual void activate() override;
	virtual void HighlightEdge() override;
	virtual void CancelHighlightEdge() override;
	virtual void HighlightTile() override;
	virtual void CancelHighlightTile() override;
	UFUNCTION()
	void OnStateChanged( const CustomGameState newState );
};
