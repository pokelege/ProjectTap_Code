// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Engine/Blueprint.h"
#include "BallPawn.generated.h"

UCLASS()
class PROJECTTAP_API ABallPawn : public APawn
{
	GENERATED_BODY()

	//these two vectors are used when the ball
	//transits to the center of a tile
	FVector transitionNormal;
	FVector lastAnchorPosition;

	class APawnCastingTrigger* trigger = nullptr;
	class UProjectTapCameraComponent* cameraComponent = nullptr;
	UMaterialInstanceDynamic* material = nullptr;	
	float currentDieTime = 0;
	bool dying = false;
	bool bInvincible = false;
	bool bTransitioning = false;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ball)
	UUserWidget* pauseMenuInstance = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ball)
	UBlueprint* pauseMenuBlueprint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
	UCurveFloat* dieSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
	UStaticMeshComponent* ballMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
	USphereComponent* ballCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
		FVector initialVelocity = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
	class UConstrainingSpringArmComponent* spring;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
	UAudioComponent* dieSound = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ball)
	float transitionSpeed = 20.0f;

	// Sets default values for this actor's properties
	ABallPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void UpdateResetTransition(float dt);

	UFUNCTION(BlueprintCallable, Category = "Ball")
	void AddVelocity(const FVector& vel, const FVector& resetPos, bool clearForce = true);

	//reset ball to the center of the tile when hit
	void TransitionBallToProperLocation(const FVector& resetPosition, const FVector& newVel);

	void ResetBallXYPosition(const FVector& position);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void FellOutOfWorld(const class UDamageType & dmgType) override;

	void Kill();

	bool isDying();

	void setInvincibility(bool invincible);

	UFUNCTION(BlueprintCallable, Category=ToggleMenu)
	void togglePauseMenu();


	void setCamera(class ABallPlayerStart* playerStart);
	UProjectTapCameraComponent* GetCamera();
};
