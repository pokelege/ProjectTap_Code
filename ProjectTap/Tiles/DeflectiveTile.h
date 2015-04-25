// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "DeflectiveTile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTAP_API ADeflectiveTile : public ATile
{
	GENERATED_BODY()
	float timer = 0.0f;
	int rotationDegreeLimit = 90;
	int currentRotation = 45;

	int GetCurrentRotation();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		float duration = 0.5f;


	ADeflectiveTile();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Highlight(bool litTile = true, bool litEdge = true);
	virtual void CancelHighlight();

	void Spin(float dt);
};
