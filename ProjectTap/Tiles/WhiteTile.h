// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "ICarriable.h"
#include "WhiteTile.generated.h"

UCLASS()
class PROJECTTAP_API AWhiteTile : public ATile, public ICarriable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWhiteTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual OffsetInfo getOffsetInfo() override;
	
};
