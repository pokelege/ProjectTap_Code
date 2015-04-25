// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "EndTile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTAP_API AEndTile : public ATile
{
	GENERATED_BODY()

	static const FName END_MESH;
	TScriptDelegate<FWeakObjectPtr> delegate;
public:
	
	AEndTile();

	virtual void Highlight(bool litTile = true, bool litEdge = true) override;

	UFUNCTION()
	void OnHit(AActor* OtherActor,
			   UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit);
};