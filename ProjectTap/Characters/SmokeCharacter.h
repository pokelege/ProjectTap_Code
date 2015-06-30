// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "SmokeCharacter.generated.h"

UCLASS()
class PROJECTTAP_API ASmokeCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smoke)
	class UParticleSystemComponent* smokeParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smoke)
	class UBehaviorTree* behavior;

	// Sets default values for this character's properties
	ASmokeCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;



};
