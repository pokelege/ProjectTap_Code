// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameMode.h"
#include "ProjectTapGameState.h"
#include "Controllers/MouseController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#define printonscreen(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)
AProjectTapGameMode::AProjectTapGameMode( const FObjectInitializer& initializer ): Super( initializer )
{
	UE_LOG( LogTemp , Warning , TEXT( "mouse" ) );
	PlayerControllerClass = AMouseController::StaticClass();
	DefaultPawnClass = 0;
	GameStateClass = AProjectTapGameState::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectTapGameMode::BeginPlay()
{
	if (UWorld* world = GetWorld())
	{
		FTransform playerStart = FindPlayerStart(0, FString("Player"))->GetTransform();
		FActorSpawnParameters params;
		//AActor* spawned = world->SpawnActor(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotation(playerStart.GetRotation());
		ball = world->SpawnActor<ABallPawn>(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotator(playerStart.GetRotation()), params);
 	}
 	GetGameState<AProjectTapGameState>()->SetMatchState(GAME_STATE_GAME_OVER);
}

void AProjectTapGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(	GetGameState<AProjectTapGameState>()->GetMatchState().IsEqual(GAME_STATE_GAME_OVER))
	{
		//todo GameOver
		printonscreen("GameOver");
	}
}

void AProjectTapGameMode::Respawn()
{
	if (ball != nullptr)
	{
		ball->BeginDestroy();
		ball = nullptr;
	}

	if (UWorld* world = GetWorld())
	{
		FTransform playerStart = FindPlayerStart(0, FString("Player"))->GetTransform();
		FActorSpawnParameters params;
		//AActor* spawned = world->SpawnActor(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotation(playerStart.GetRotation());
		ball = world->SpawnActor<ABallPawn>(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotator(playerStart.GetRotation()), params);
	}
}