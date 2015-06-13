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
	class ABallPawn* targetBall;

	FVector targetBallLastPosition;
	float currentAnimationTime = 0;
	float ballToSocketCurveDuration = 0;
	float transportScalingCurveDuration = 0;
	bool transporting = false;
	void StartTransporting();
	void PlayTransport(const float& DeltaTime);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level)
	FName loadLevelName = FName("MainMenu");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Transport)
	UCurveVector* ballToSocketCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Transport)
	UCurveVector* transportScalingCurve;

	AEndTile();

	virtual void Tick( float DeltaTime ) override;

	UFUNCTION()
	void OnBeginHit(class AActor* OtherActor,
					class UPrimitiveComponent* OtherComp,
					FVector NormalImpulse,
					const FHitResult& Hit);
};