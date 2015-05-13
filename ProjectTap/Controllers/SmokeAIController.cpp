// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "SmokeAIController.h"
#include "Pawns/SmokePawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
ASmokeAIController::ASmokeAIController(const FObjectInitializer& ObjectInitializer) : AAIController(ObjectInitializer)
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BehaviorComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior"));
}

void ASmokeAIController::Possess(APawn *pawn)
{
	Super::Possess(pawn);
	ASmokePawn* smokePawn = Cast<ASmokePawn>(pawn);
	if(smokePawn != nullptr && smokePawn->behavior != nullptr)
	{
		BlackboardComponent->InitializeBlackboard(smokePawn->behavior->BlackboardAsset);
		BehaviorComponent->StartTree(smokePawn->behavior);
	}
}

void ASmokeAIController::SetEnemy(APawn *pawn)
{

}
