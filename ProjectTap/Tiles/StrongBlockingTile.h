// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "StrongBlockingTile.generated.h"

UCLASS()
class PROJECTTAP_API AStrongBlockingTile : public ATile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStrongBlockingTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
