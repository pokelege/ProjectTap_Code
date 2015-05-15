// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "KillBallBTService.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API UKillBallBTService : public UBTService
{
	GENERATED_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds ) override;
};
