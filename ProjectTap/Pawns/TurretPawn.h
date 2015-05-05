// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "TurretPawn.generated.h"

UCLASS()
class PROJECTTAP_API ATurretPawn : public APawn
{
	GENERATED_BODY()

	static const FName BASE_MESH;
	static const FName GUN_MESH;
	FVector nozzleLocal;
	FVector direction;
	float currentFireCooldown = 0;
	const float MAX_HEALTH = 10.0f;
	float current_hp = MAX_HEALTH;

	void UpdateLaserTag(float dt);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	bool activated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	UStaticMeshComponent* TurretGunMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	UParticleSystemComponent* laserTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	float FOV = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	float maxDistance = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	float updateInterval = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	float fireRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	float bulletForce = 1000.0f;


	// Sets default values for this pawn's properties
	ATurretPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void AttemptToFire(const float& DeltaTime);

	bool FoundPlayerToHit();

	void Fire();	

	void TakeDamage(float deathDuration);

	void kill();
};
