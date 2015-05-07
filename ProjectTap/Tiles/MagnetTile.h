// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "MagnetTile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTAP_API AMagnetTile : public ATile
{
	GENERATED_BODY()
	
	
	
	virtual void Tick( float DeltaTime ) override;
};
