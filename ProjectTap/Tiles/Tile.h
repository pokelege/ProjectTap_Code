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
  FVector original;
  bool activated = false;
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UStaticMeshComponent* TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		float move_speed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		float move_distance_tolerance = 500.0f;
	
	// Sets default values for this actor's properties
	ATile( const FObjectInitializer& initializer );
	
	virtual void activate();

	virtual void deactivate();

	virtual bool isActivated();
	
	virtual void BeginPlay();
	virtual void Tick( float DeltaTime );
};