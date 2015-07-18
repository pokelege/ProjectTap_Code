// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "SmokeAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Pawns/SmokePawn.h"
#include "Characters/SmokeCharacter.h"
#include "ProjectTapGameState.h"
#include "Pawns/BallPawn.h"

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
	else
	{
		ASmokeCharacter* smokeCharacter = Cast<ASmokeCharacter>(pawn);
		if(smokeCharacter != nullptr && smokeCharacter->behavior != nullptr)
		{
			BlackboardComponent->InitializeBlackboard(*smokeCharacter->behavior->BlackboardAsset);
			BehaviorComponent->StartTree(*smokeCharacter->behavior);
		}
	}
}


bool ASmokeAIController::SetEnemy()
{
	ASmokePawn* smokePawn = Cast<ASmokePawn>(GetPawn());
	if(smokePawn != nullptr && smokePawn->behavior != nullptr)
	{
		AProjectTapGameState* gamestate = Cast<AProjectTapGameState>(GetWorld()->GetGameState());
		if(gamestate != nullptr && gamestate->GetPlayer() != nullptr)
		{
			BlackboardComponent->SetValueAsObject(FName("Enemy"), gamestate->GetPlayer());
			BlackboardComponent->SetValueAsVector(FName("Destination"), gamestate->GetPlayer()->GetActorLocation());
			return true;
		}
	}
	else
	{
		ASmokeCharacter* smokeCharacter = Cast<ASmokeCharacter>(GetPawn());
		if(smokeCharacter != nullptr && smokeCharacter->behavior != nullptr)
		{
			AProjectTapGameState* gamestate = Cast<AProjectTapGameState>(GetWorld()->GetGameState());
			if(gamestate != nullptr && gamestate->GetPlayer() != nullptr)
			{
				BlackboardComponent->SetValueAsObject(FName("Enemy"), gamestate->GetPlayer());
				BlackboardComponent->SetValueAsVector(FName("Destination"), gamestate->GetPlayer()->GetActorLocation());
				return true;
			}
		}
	}
	return false;
}

bool ASmokeAIController::KillEnemy()
{
	AProjectTapGameState* gamestate = Cast<AProjectTapGameState>(GetWorld()->GetGameState());
	if(gamestate != nullptr &&  gamestate->GetPlayer() != nullptr)
	{
		gamestate->GetPlayer()->Kill();
		return true;
	}
	else
		return false;
}

void ASmokeAIController::UpdateCameraSaturation()
{
	AProjectTapGameState* gamestate = Cast<AProjectTapGameState>(GetWorld()->GetGameState());

	float length = (GetPawn()->GetActorLocation() - gamestate->GetPlayer()->GetActorLocation()).Size();
	float newColor = FMath::Clamp<float>((length - gamestate->aiMinDistance) / (gamestate->aiMaxDistance - gamestate->aiMinDistance), 0, 1);
}
