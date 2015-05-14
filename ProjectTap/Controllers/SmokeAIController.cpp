// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "SmokeAIController.h"
#include "Pawns/SmokePawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ProjectTapGameState.h"
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
		BlackboardComponent->InitializeBlackboard(*smokePawn->behavior->BlackboardAsset);
		BehaviorComponent->StartTree(*smokePawn->behavior);
	}
}


void ASmokeAIController::SetEnemy()
{
	ASmokePawn* smokePawn = Cast<ASmokePawn>(GetPawn());
	if(smokePawn != nullptr && smokePawn->behavior != nullptr)
	{
		AProjectTapGameState* gamestate = Cast<AProjectTapGameState>(GetWorld()->GetGameState());
		if(gamestate != nullptr)
		{
			BlackboardComponent->SetValueAsObject(FName("Enemy"), gamestate->CurrentPawn);
		}
	}
}