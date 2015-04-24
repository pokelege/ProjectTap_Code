// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Laser.generated.h"

UCLASS()
class PROJECTTAP_API ALaser : public AActor
{
	GENERATED_BODY()

	ALaser* nextLaser = nullptr;
	float timer = .0f;
	float elapseTime = 0.1f;
	UParticleSystem* particleAsset;
	const int MAX_DEPTH = 5;
	int currentDepth = 0;

	void checkLaserCollisions(float dt);
	
	void SpawnSubLaser(const FVector& start, const FVector& normal);
	
	FVector reflect(const FVector& v1, const FVector& v2);
	
	void SetLaserDepth(unsigned i);
	
	bool CanSpawnSubLaser();

	FHitResult Collide();
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Laser)
	UParticleSystemComponent* laserParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Laser)
	FVector direction = FVector(1.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Laser)
	float length = 1000.0f;

	// Sets default values for this actor's properties
	ALaser();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SetLocation(const FVector& vector);
	
};
