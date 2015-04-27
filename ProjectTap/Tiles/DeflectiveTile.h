// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "DeflectiveTile.generated.h"

UENUM(BlueprintType)
enum class DeflectiveTileType : uint8
{
	HORIZONTAL,
	VERTICAL_NORMAL_X,
	VERTICAL_NORMAL_Y
};

/**
 * 
 */
UCLASS()
class PROJECTTAP_API ADeflectiveTile : public ATile
{
	GENERATED_BODY()
	float timer = 0.0f;
	float rotationDegreeLimit = 90;
	float currentRotation = 45;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		float rotationDuration = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		DeflectiveTileType type;

	ADeflectiveTile();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

    void Highlight(bool litTile = true, bool litEdge = true) override;
    void CancelHighlight() override;

	void activate() override;
	void deactivate() override;


	void Spin(float dt);
};
