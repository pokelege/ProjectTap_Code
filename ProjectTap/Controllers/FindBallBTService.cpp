// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "FindBallBTService.h"
#include "SmokeAIController.h"





void UFindBallBTService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
	ASmokeAIController* ai = Cast<ASmokeAIController>(OwnerComp.GetAIOwner());
	if(ai != nullptr)
	{
		ai->SetEnemy();
	}
}
