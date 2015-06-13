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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level)
	FName loadLevelName = FName("MainMenu");

	AEndTile();

	UFUNCTION()
	void OnBeginHit(class AActor* OtherActor,
					class UPrimitiveComponent* OtherComp,
					FVector NormalImpulse,
					const FHitResult& Hit);
};