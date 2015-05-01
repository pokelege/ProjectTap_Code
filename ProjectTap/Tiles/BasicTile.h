// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BasicTile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTAP_API ABasicTile : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UStaticMeshComponent* TileMesh = nullptr;

	ABasicTile();
	
};
