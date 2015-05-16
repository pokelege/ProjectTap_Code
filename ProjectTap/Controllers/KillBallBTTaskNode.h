// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "KillBallBTTaskNode.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API UKillBallBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()


public:
	virtual PRAGMA_POP EBTNodeResult::Type ExecuteTask
	(
	  UBehaviorTreeComponent & OwnerComp,
	  uint8 * NodeMemory
	);

};
