// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "KillBallBTTaskNode.h"
#include "SmokeAIController.h"





EBTNodeResult::Type UKillBallBTTaskNode::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	ASmokeAIController* ai = Cast<ASmokeAIController>(OwnerComp.GetAIOwner());
	if(ai != nullptr)
	{
		if(ai->KillEnemy()) return EBTNodeResult::Type::Succeeded;
	}
	return EBTNodeResult::Type::Failed;
}
