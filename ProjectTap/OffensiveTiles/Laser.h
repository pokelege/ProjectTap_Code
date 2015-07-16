// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tiles/ICarriable.h"
#include "CustomGameState.h"
#include "Laser.generated.h"

UCLASS()
class PROJECTTAP_API ALaser : public AActor, public ICarriable
{
	GENERATED_BODY()

	ALaser* nextLaser = nullptr;
	FVector currHitPoint;
	float timer = .0f;
	float elapseTime = 1 / 30.0f;
	int currentDepth = 0;
	const int MAX_DEPTH = 8;
	bool canHitBall = true;
	FDelegateHandle OnGameStateChangedDelegateHandle;
	void checkLaserCollisions(float dt);

	void SpawnSubLaser(const FVector& start, const FVector& normal);

	FVector reflect(const FVector& v1, const FVector& v2);

	void SetLaserDepth(unsigned i);

	bool CanSpawnSubLaser();

	void KillSubLaser();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Laser)
	class UEmptyComponent* root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Laser)
		UStaticMeshComponent* mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Laser)
		UParticleSystemComponent* laserParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Laser)
		UParticleSystemComponent* laserSparkParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Laser)
		UArrowComponent* debugArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Laser)
		FVector dir = FVector(1.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Laser)
		float length = 1000000.0f;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
	UAudioComponent* laserEmitSound = nullptr;
	// Sets default values for this actor's properties
	ALaser();

	UFUNCTION()
		void OnStateChanged( const CustomGameState newState );

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	OffsetInfo getOffsetInfo() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//this method will use the ray traced hit point as laser end point
	void SetLaserLocationWithDefaultHitLocation(const FVector& location);

	//this method will set the ray to a location with infinite length and given direction
	void SetLaserLocationWithDefaultDirection(const FVector& location);

};
