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

  ABlockingTile(const FObjectInitializer& initializer );
  
  virtual void BeginPlay() override;
  virtual void Tick( float DeltaTime ) override;
};
