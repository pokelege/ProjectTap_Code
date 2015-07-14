// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Editor/UMGEditor/Public/WidgetBlueprint.h"
#include "BallPawn.generated.h"

UCLASS()
class PROJECTTAP_API ABallPawn : public APawn
{
	GENERATED_BODY()

	class APawnCastingTrigger* trigger = nullptr;

	bool bInvincible = false;

	class UProjectTapCameraComponent* cameraComponent = nullptr;
	UMaterialInstanceDynamic* material = nullptr;
	UUserWidget* pauseMenuInstance = nullptr;
	UWidgetBlueprint* pauseMenuBlueprint = nullptr;
	bool dying = false;
	float currentDieTime = 0;

public:
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

	// Sets default values for this actor's properties
	ABallPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "Ball")
	void AddVelocity(const FVector& vel, const FVector& resetPos, bool clearForce = true);

	//reset ball to the center of the tile when hit
	void ResetBallXYPosition(const FVector& resetPosition, const FVector& newVel);

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
