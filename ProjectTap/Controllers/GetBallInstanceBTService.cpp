// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GetBallInstanceBTService.h"
#include "SmokeAIController.h"





void UGetBallInstanceBTService::TickNode( UBehaviorTreeComponent &OwnerComp , uint8 *NodeMemory , float DeltaSeconds )
{
	ASmokeAIController* ai = Cast<ASmokeAIController>( OwnerComp.GetAIOwner() );
	if ( ai != nullptr )
	{
		ai->SetEnemy();
	}
}
