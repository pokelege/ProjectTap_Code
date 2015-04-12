// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
/// <summary>
/// The base tile
/// </summary>
class PROJECTTAP_API ATile : public AActor
{
	GENERATED_BODY()

protected:
  bool activated = false;
  FVector original;
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UStaticMeshComponent* TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UBoxComponent* BoxCollision;
	// Sets default values for this actor's properties
	ATile( const FObjectInitializer& initializer );
	
	virtual void activate();

	virtual void deactivate();
	
	virtual void BeginPlay();
	virtual void Tick( float DeltaTime );
};