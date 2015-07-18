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
	FDelegateHandle OnGameStateChangedDelegateHandle;
	FVector targetBallLastPosition;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Level )
		FName loadLevelName = FName( "MainMenu" );
private:
	TArray<USoundBase*> sounds;
	UAudioComponent* audioPlayer = nullptr;
	UCurveFloat* soundToPlayCurve = nullptr;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Transport )
		UCurveVector* ballToSocketCurve = nullptr;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Transport )
		UCurveVector* transportScalingCurve = nullptr;
private:
	class ABallPawn* targetBall = nullptr;
	float currentAnimationTime = 0;
	float ballToSocketCurveDuration = 0;
	float transportScalingCurveDuration = 0;
	bool transporting = false;
	bool CanTransport = false;
public:

	AEndTile();
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	virtual void Tick( float DeltaTime ) override;

	UFUNCTION()
		void OnGameStateChanged( const CustomGameState gameState );

	UFUNCTION()
	void OnBeginHit(class AActor* OtherActor,
					class UPrimitiveComponent* OtherComp,
					FVector NormalImpulse,
					const FHitResult& Hit);
private:
	void StartTransporting();
	void PlayTransport( const float& DeltaTime );
};
