// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BallPawn.generated.h"

UCLASS()
class PROJECTTAP_API ABallPawn : public APawn
{
	GENERATED_BODY()
	
	class APawnCastingTrigger* trigger = nullptr;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
	UStaticMeshComponent* ballMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
	USphereComponent* ballCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
		FVector initialVelocity = FVector(0.0f, 0.0f, 0.0f);

	// Sets default values for this actor's properties
	ABallPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "Ball")
	void AddVelocity(const FVector& vel, bool clearForce = true);

	//reset ball to the center of the tile when hit
	void ResetBallXYPosition(const FVector& position);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void FellOutOfWorld(const class UDamageType & dmgType) override;

	void Kill();	


};
