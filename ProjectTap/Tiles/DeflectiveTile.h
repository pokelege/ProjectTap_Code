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
private:
	static const GroundableInfo groundableInfo;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		USceneComponent* frameCollisionsComponent = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
		UAudioComponent* ballDeflectSound = nullptr;
private:
	float timer = 0.0f;
	float rotationDegreeLimit = 90;
	float accum = 0;
	float edgeHighlightTimer = 0.0f;
	float edgeHighlightDuration = 0.0f;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		float rotationDuration = 2.0f;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		float additionalDeflectingSpeed = 300.0f;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		float currentRotation = 45;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		DeflectiveTileType type;
private:
	bool ballCanTouch = true;
public:

	ADeflectiveTile();

	UFUNCTION()
	void OnHit(class AActor* OtherActor,
			   class UPrimitiveComponent* OtherComp,
			   FVector NormalImpulse,
			   const FHitResult& Hit);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void turnOffHighlight(bool offTile = true, bool offEdge = true) override;
	virtual OffsetInfo getOffsetInfo() override;

	//for a 3D vector, this function will find the axis with largest value
	//and make the other two 0
	//@Param resetValueToOne: if true, set the largest value to 1
	static FVector clampShortAxis(const FVector& vec, bool resetValueToOne = false);

	void HighlightEdgeForDuration(float duration);

	void activate() override;
	void deactivate() override;

	void Spin(float dt);
	void UpdateEdgeHighlight( float dt );
	virtual const struct GroundableInfo* GetGroundableInfo() const override;
};
