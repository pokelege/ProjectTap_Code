// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/BlockingTileBase.h"
#include "GroupedBlockingTile.generated.h"

UCLASS()
class PROJECTTAP_API AGroupedBlockingTile : public ABlockingTileBase
{
	GENERATED_BODY()
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		float activation_time = .5f;

	// Sets default values for this actor's properties
	AGroupedBlockingTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void deactivate() override;
	
};
