// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "Pawns/BallPawn.h"
#include "Ramp.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTTAP_API ARamp : public ATile
{
	GENERATED_BODY()
	static const FName RAMP_MESH_PATH;
	static const FName RAMP_TOP_MESH_PATH;
	static const FName RAMP_CURVE_PATH;
	
	float time = 0;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UCurveFloat* rotationSequence;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
// 	UBoxComponent* BallTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UStaticMeshComponent* TopMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	float forceMultiplier;

	ARamp();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

	virtual void activate() override;

	void BoostBall();


};
