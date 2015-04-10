// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "StrongTile.generated.h"

UCLASS()
class PROJECTTAP_API AStrongTile : public ATile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStrongTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
