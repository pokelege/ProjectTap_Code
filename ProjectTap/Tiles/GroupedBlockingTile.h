// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "GroupedBlockingTile.generated.h"

UCLASS()
class PROJECTTAP_API AGroupedBlockingTile : public ATile
{
	GENERATED_BODY()
	float time_counter = 0.0f;
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		float activation_time = 2.0f;

	// Sets default values for this actor's properties
	AGroupedBlockingTile(const FObjectInitializer& initializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void deactivate() override;
	
};
