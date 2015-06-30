// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "UpdateCameraSaturationBTService.h"
#include "SmokeAIController.h"



void UUpdateCameraSaturationBTService::TickNode
(
	UBehaviorTreeComponent & OwnerComp,
	uint8 * NodeMemory,
	float DeltaSeconds
)
{
	ASmokeAIController* ai = Cast<ASmokeAIController>(OwnerComp.GetAIOwner());
	if(ai != nullptr)
	{
		ai->UpdateCameraSaturation();
	}
}
