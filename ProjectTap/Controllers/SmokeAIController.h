// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "SmokeAIController.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API ASmokeAIController : public AAIController
{
	GENERATED_BODY()

	class APawn* enemy;
public:

	ASmokeAIController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY()
	class UBehaviorTreeComponent* BehaviorComponent;

	virtual void Possess(class APawn* pawn) override;

	void SetEnemy(class APawn* pawn);

};
