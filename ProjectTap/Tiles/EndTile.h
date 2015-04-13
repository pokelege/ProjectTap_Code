// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "EndTile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTAP_API AEndTile : public ATile
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UBoxComponent* BallCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	AActor* ball;
	
	virtual void Tick( float DeltaTime ) override;
};
