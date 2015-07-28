// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PawnCastingTrigger.h"
#include "BallPawn.generated.h"

UCLASS()
class PROJECTTAP_API ABallPawn : public APawn
{
	GENERATED_BODY()

	//these two vectors are used when the ball
	//transits to the center of a tile
	FVector transitionNormal;
	FVector lastAnchorPosition;		

	void SpawnCastingTrigger(BallCastType type);
private:
	friend class ADeflectiveTile;
	FVector currentNormal;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Ball )
		FVector initialVelocity = FVector( 0.0f , 0.0f , 0.0f );
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
		UAudioComponent* dieSound = nullptr;
	UPROPERTY( VisibleAnywhere , BlueprintReadWrite , Category = Ball )
		class UUserWidget* pauseMenuInstance = nullptr;

	UPROPERTY( VisibleAnywhere , BlueprintReadWrite , Category = Ball )
		class UClass* pauseMenuBlueprint = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Ball )
		UCurveFloat* dieSequence;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Ball )
		UStaticMeshComponent* ballMesh;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Ball )
		USphereComponent* ballCollision;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Ball )
	class UConstrainingSpringArmComponent* spring;
private:
	class APawnCastingTrigger* rampTrigger = nullptr;
	class APawnCastingTrigger* blockingTrigger = nullptr;
	class UProjectTapCameraComponent* cameraComponent = nullptr;
	UMaterialInstanceDynamic* material = nullptr;	
	float currentDieTime = 0;
public:
	float currentTransitionSpeed = 200.0f;
	const float DEFUALT_TRANSITION_SPEED = 200.0f;
private:
	bool dying = false;
	bool bInvincible = false;
	bool bTransitioning = false;
	bool bTransitionFinishNextFrame = false;

public:
	// Sets default values for this actor's properties
	ABallPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void UpdateResetTransition(float dt);

	UFUNCTION(BlueprintCallable, Category = "Ball")

	void AddVelocity(const FVector& vel, bool clearForce = true);
	void AddVelocity(const FVector& vel, const FVector& resetPos, bool clearForce = true);

	//reset ball to the center of the tile when hit
	//@param transitionSpeed: as the name says, however, it will be reset to default speed after next transition is finished,
	//it will not change the default speed
	void TransitionBallToProperLocation(const FVector& resetPosition, const FVector& newVelDir,float transitionSpeed = 300.0f);

	void ResetBallXYPosition(const FVector& position);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void FellOutOfWorld(const class UDamageType & dmgType) override;

	void Kill();

	bool isDying();

	void setInvincibility(bool invincible);

	UFUNCTION(BlueprintCallable, Category=ToggleMenu)
	void togglePauseMenu();

	UFUNCTION()
	void OnHit(class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	void setCamera(class ABallPlayerStart* playerStart);
	UProjectTapCameraComponent* GetCamera();


};
