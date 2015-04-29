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
  bool enabled = true;
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UStaticMeshComponent* TileMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UBoxComponent* BoxCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Color)
	UMaterialInstanceDynamic* material = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Color)
		FLinearColor baseColor = FLinearColor(1.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Color)
		FLinearColor glowColor = FLinearColor(0.0f, 1.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Color)
	float glowPower = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Color)
		FLinearColor baseColorHighlighted = FLinearColor(5.f, 0.0f, .0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Color)
		FLinearColor glowColorHighlighted = FLinearColor(5.0f, 5.0f, 5.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Color)
		float glowPowerHighlighted = 20.0f;

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

	virtual void Enable();
	virtual void Disable();
	virtual bool IsEnabled();
	
	virtual void BeginPlay();
	virtual void Tick( float DeltaTime );

	virtual void Highlight(bool litTile = true, bool litEdge = true);
	virtual void CancelHighlight();
	virtual void turnOffHighlight();

};