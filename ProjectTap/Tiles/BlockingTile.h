// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "BlockingTile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTAP_API ABlockingTile : public ATile
{
	GENERATED_BODY()
public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
  float original = 550.0f;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
  float move_speed = 800.0f;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
  float move_distance_tolerance = 500.0f;
  
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tile)
  UBoxComponent* boxCollision;
  
  ABlockingTile(const FObjectInitializer& initializer );
  
  virtual void BeginPlay() override;
  virtual void Tick( float DeltaTime ) override;
};
