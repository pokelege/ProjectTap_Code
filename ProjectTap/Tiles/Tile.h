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
	UStaticMeshComponent* TileMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		UBoxComponent* BoxCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		UMaterialInstanceDynamic* material = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		float move_speed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		float move_distance_tolerance = 100.0f;
	
	// Sets default values for this actor's properties
	ATile(  );
	
	UFUNCTION(BlueprintCallable, Category = "Tile")
	virtual void activate();

	UFUNCTION(BlueprintCallable, Category = "Tile")
	virtual void deactivate();

	virtual bool isActivated();
	
	virtual void BeginPlay();
	virtual void Tick( float DeltaTime );

	virtual void MaterialHighlight();
	virtual void CancelHighlight();
};