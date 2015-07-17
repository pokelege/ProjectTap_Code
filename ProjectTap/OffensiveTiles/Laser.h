// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tiles/ICarriable.h"
#include "CustomGameState.h"
#include "Laser.generated.h"

UCLASS()
class PROJECTTAP_API ALaser : public AActor , public ICarriable
{
	GENERATED_BODY()

		FDelegateHandle OnGameStateChangedDelegateHandle;
	FVector currHitPoint;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Laser )
		FVector dir = FVector( 1.0f , 0.0f , 0.0f );
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Laser )
	class UEmptyComponent* root = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Laser )
		UStaticMeshComponent* mesh = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Laser )
		UParticleSystemComponent* laserParticle = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Laser )
		UParticleSystemComponent* laserSparkParticle = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Laser )
		UArrowComponent* debugArrow = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
		UAudioComponent* laserEmitSound = nullptr;
private:
	ALaser* nextLaser = nullptr;
	static const int MAX_DEPTH;
	int currentDepth = 0;
	float timer = .0f;
	float elapseTime = 1 / 30.0f;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Laser )
		float length = 1000000.0f;
private:
	bool canHitBall = true;
public:
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
	void SetLaserLocationWithDefaultHitLocation( const FVector& location );

	//this method will set the ray to a location with infinite length and given direction
	void SetLaserLocationWithDefaultDirection( const FVector& location );
private:
	void checkLaserCollisions( float dt );

	void SpawnSubLaser( const FVector& start , const FVector& normal );

	FVector reflect( const FVector& v1 , const FVector& v2 );

	void SetLaserDepth( unsigned i );

	bool CanSpawnSubLaser();

	void KillSubLaser();
};
