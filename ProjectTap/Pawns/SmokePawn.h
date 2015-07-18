// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "SmokePawn.generated.h"

UCLASS()
class PROJECTTAP_API ASmokePawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smoke)
	class UBehaviorTree* behavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smoke)
	class UFloatingPawnMovement* movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smoke)
	class USphereComponent* collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smoke)
	class UParticleSystemComponent* smokeParticleSystem;
	// Sets default values for this pawn's properties
	ASmokePawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;
};
