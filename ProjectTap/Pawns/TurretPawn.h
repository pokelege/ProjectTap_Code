// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Tiles/IGroundable.h"
#include "TurretPawn.generated.h"

UCLASS()
class PROJECTTAP_API ATurretPawn : public APawn, public IGroundable
{
	GENERATED_BODY()

	static const FName BASE_MESH;
	static const FName GUN_MESH;
	static const float MAX_HEALTH;
	static const GroundableInfo groundableInfo;

	FDelegateHandle OnPlayerChangedDelegateHandle;
	FVector nozzleLocal;
	FVector nozzleLocalUpdatable;
	FVector direction;
	FRotator regularRotation;
	class ABallPawn* target = nullptr;
	UAudioComponent* explosionSound = nullptr;
	UAudioComponent* nozzleSound = nullptr;
	USoundBase* fireSound = nullptr;
	USoundBase* lockSound = nullptr;
	USoundBase* unlockSound = nullptr;
	UStaticMeshComponent* TurretGunMesh;
	UParticleSystemComponent* laserTag;
	UParticleSystemComponent* explosionParticle;
public:

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Turret )
		float fireDelay = 0.5f;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Turret )
		float maxErrorToShoot = 0.01f;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Turret )
		float ballSightedRotateSpeed = 10.0f;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Turret )
		float idleRotateSpeed = 1.0f;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Turret )
		float rotation = 60.0f;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Turret )
		float FOV = 60.0f;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Turret )
		float maxDistance = 300.0f;

private:
	float current_hp = MAX_HEALTH;
	float currentTime = 0;
	float currentFireDelayTime = 0;
	bool died = false;
	bool wasLocked = false;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	bool activated = true;
	// Sets default values for this pawn's properties
	ATurretPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	bool FoundPlayerToHit();

	bool CanRotateToPlayer();

	bool RotateToPlayer( const float& DeltaTime );

	bool LockPlayer(const float& DeltaTime);

	void Fire();

	void Damage(float deathDuration);

	UFUNCTION(BlueprintCallable, Category="Turret")
	void Kill();

	UFUNCTION()
		void OnPlayerChanged( ABallPawn* newPlayer );

	virtual const struct GroundableInfo* GetGroundableInfo() const override;
private:
	void UpdateLaserTag( float dt );
	void UpdateTurretDamage( float dt );
};
