// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "Pawns/BallPawn.h"
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
class PROJECTTAP_API ARamp : public ATile
{
	GENERATED_BODY()

	static const FName RAMP_MESH_PATH;
	static const FName RAMP_CURVE_PATH;
	FScriptDelegate pawnIn;
	FScriptDelegate pawnOut;
	float time = 0;
	ABallPawn* ball;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	Direction rotationDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UCurveFloat* rotationSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UBoxComponent* boxTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	float forceMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	float additionalZForce = 0.0f;

	ARamp();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

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
