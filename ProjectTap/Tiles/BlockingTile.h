// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/BlockingTileBase.h"
#include "BlockingTile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTAP_API ABlockingTile : public ABlockingTileBase
{
	GENERATED_BODY()
	float time_counter = 0.0f;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		float activation_time = 3.0f;
  ABlockingTile( );
  
   virtual void BeginPlay() override;
   virtual void Tick( float DeltaTime ) override;

};
