// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "ICarriable.h"
#include "DeflectiveTile.generated.h"

UENUM(BlueprintType)
enum class DeflectiveTileType : uint8
{
	HORIZONTAL,
	VERTICAL_NORMAL_X,
	VERTICAL_NORMAL_Y
};

/**
 *
 */
UCLASS()
class PROJECTTAP_API ADeflectiveTile : public ATile, public ICarriable
{
	GENERATED_BODY()
	float timer = 0.0f;
	float rotationDegreeLimit = 90;
	float accum = 0;
	float edgeHighlightTimer = 0.0f;
	float edgeHighlightDuration = 0.0f;
	bool ballCanTouch = true;

	void UpdateEdgeHighlight(float dt);
	//only keep the axis with biggest value
	//others will be set to 0
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		float rotationDuration = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		float deflectingSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		DeflectiveTileType type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	float currentRotation = 45;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		USceneComponent* frameCollisionsComponent = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
		UAudioComponent* ballDeflectSound = nullptr;
	ADeflectiveTile();

	UFUNCTION()
	void OnHit(class AActor* OtherActor,
			   class UPrimitiveComponent* OtherComp,
			   FVector NormalImpulse,
			   const FHitResult& Hit);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual OffsetInfo getOffsetInfo() override;

	static FVector clampShortAxis(const FVector& vec);


	void HighlightEdgeForDuration(float duration);

	void activate() override;
	void deactivate() override;

	void Spin(float dt);
};
