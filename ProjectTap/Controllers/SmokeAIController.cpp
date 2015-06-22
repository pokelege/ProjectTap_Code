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


bool ASmokeAIController::SetEnemy()
{
	ASmokePawn* smokePawn = Cast<ASmokePawn>(GetPawn());
	if(smokePawn != nullptr && smokePawn->behavior != nullptr)
	{
		AProjectTapGameState* gamestate = Cast<AProjectTapGameState>(GetWorld()->GetGameState());
		if(gamestate != nullptr && gamestate->CurrentPawn != nullptr)
		{
			BlackboardComponent->SetValueAsObject(FName("Enemy"), gamestate->CurrentPawn);
			BlackboardComponent->SetValueAsVector(FName("Destination"), gamestate->CurrentPawn->GetActorLocation());
			return true;
		}
	}
	return false;
}

bool ASmokeAIController::KillEnemy()
{
	AProjectTapGameState* gamestate = Cast<AProjectTapGameState>(GetWorld()->GetGameState());
	if(gamestate != nullptr &&  gamestate->CurrentPawn != nullptr)
	{
		gamestate->CurrentPawn->Kill();
		return true;
	}
	else
		return false;
}

void ASmokeAIController::UpdateCameraSaturation()
{
	AProjectTapGameState* gamestate = Cast<AProjectTapGameState>(GetWorld()->GetGameState());

	float length = (GetPawn()->GetActorLocation() - gamestate->CurrentPawn->GetActorLocation()).Size();
	float newColor = FMath::Clamp<float>((length - gamestate->aiMinDistance) / (gamestate->aiMaxDistance - gamestate->aiMinDistance), 0, 1);
	auto cameraToChangeTest = gamestate->CurrentCamera->GetComponentByClass(UCameraComponent::StaticClass());
	auto cameraToChange = Cast<UCameraComponent>(cameraToChangeTest);
	if(cameraToChange)
	{
		cameraToChange->PostProcessSettings.bOverride_FilmSaturation = true;
		cameraToChange->PostProcessSettings.FilmSaturation = newColor;
	}
}
