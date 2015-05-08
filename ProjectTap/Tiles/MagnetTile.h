// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "MagnetTile.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API AMagnetTile : public ATile
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnet)
	float length = 99999.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnet)
	float force = 99999.0f;

	AMagnetTile();
	virtual void Tick( float DeltaTime ) override;

	virtual void ReceiveHit
	(
		class UPrimitiveComponent * MyComp,
		AActor * Other,
		class UPrimitiveComponent * OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult & Hit
	);
};
