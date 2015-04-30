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
	ABallPawn* ball = nullptr;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UCurveFloat* rotationSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UBoxComponent* boxTrigger;

    ABaseRampTile();

	virtual void Tick( float DeltaTime ) override;

	virtual void Highlight(bool litTile = true, bool litEdge = true) override;

	virtual void activate() override;

	UFUNCTION()
	void OnBeginTriggerOverlap(AActor* OtherActor,
							   UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex,
							bool bFromSweep,
							const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndTriggerOverlap(AActor* OtherActor,
							 UPrimitiveComponent* OtherComp,
						  int32 OtherBodyIndex);
};
