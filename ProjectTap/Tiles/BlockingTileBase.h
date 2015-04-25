// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "BlockingTileBase.generated.h"

UCLASS()
class PROJECTTAP_API ABlockingTileBase : public ATile
{
	GENERATED_BODY()

protected:
	virtual void lerpMaterialColorForCoolDown(const float& beta);
	
public:	

	// Sets default values for this actor's properties
	ABlockingTileBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds)override;

	
};
