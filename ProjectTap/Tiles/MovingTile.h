// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "MovingTile.generated.h"

UCLASS()
class PROJECTTAP_API AMovingTile : public ATile
{
	GENERATED_BODY()
	
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	TArray<FVector> path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	float speed = 100.0f;

	// Sets default values for this actor's properties
	AMovingTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
