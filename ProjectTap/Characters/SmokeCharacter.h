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
};
