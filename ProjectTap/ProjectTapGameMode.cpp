// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameMode.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameState.h"
#include "Controllers/MouseController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Pawns/BallPawn.h"
#include "Pawns/BallPlayerStart.h"
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
	if ( UWorld* world = GetWorld() )
	{
		AActor* playerStart = FindPlayerStart( 0, FString( "Player" ) );
		FTransform playerTransform = playerStart->GetTransform();
		if ( ABallPlayerStart* realPlayerStart = Cast<ABallPlayerStart>( playerStart ) )
		{
			FActorSpawnParameters params;
			//AActor* spawned = world->SpawnActor(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotation(playerStart.GetRotation());
			ball = world->SpawnActor<ABallPawn>( ABallPawn::StaticClass(), playerTransform.GetTranslation(), FRotator( playerTransform.GetRotation() ), params );
			
			if (ball != nullptr)
			{
				ball->AddVelocity(realPlayerStart->initialVelocity);
			}
		}
		else
		{
			FActorSpawnParameters params;
			//AActor* spawned = world->SpawnActor(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotation(playerStart.GetRotation());
			ball = world->SpawnActor<ABallPawn>( ABallPawn::StaticClass(), playerTransform.GetTranslation(), FRotator( playerTransform.GetRotation() ), params );
		}

		GetGameState<AProjectTapGameState>()->CurrentPawn = ball;
	}
	GetGameState<AProjectTapGameState>()->SetState(AProjectTapGameState::GAME_STATE_PLAYING);

}

void AProjectTapGameMode::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (GetGameState<AProjectTapGameState>()->GetState() == AProjectTapGameState::GAME_STATE_GAME_OVER )
	{
		//todo GameOver
		printonscreen( "GameOver" );
		Respawn();
	}
	else if (GetGameState<AProjectTapGameState>()->GetState() == AProjectTapGameState::GAME_STATE_WIN)
	{
		printonscreen( "You win!" );
	}
}

void AProjectTapGameMode::Respawn()
{
	if ( UWorld* world = GetWorld() )
	{
		AActor* playerStart = FindPlayerStart( 0, FString( "Player" ) );
		FTransform playerTransform = playerStart->GetTransform();
		if ( ABallPlayerStart* realPlayerStart = Cast<ABallPlayerStart>( playerStart ) )
		{
			FActorSpawnParameters params;
			ball = world->SpawnActor<ABallPawn>( ABallPawn::StaticClass(), playerTransform.GetTranslation(), FRotator( playerTransform.GetRotation() ), params );
			if (ball != nullptr)
			{
				ball->AddVelocity(realPlayerStart->initialVelocity);
			}
		}
		else
		{
			FActorSpawnParameters params;
			//AActor* spawned = world->SpawnActor(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotation(playerStart.GetRotation());
			ball = world->SpawnActor<ABallPawn>( ABallPawn::StaticClass(), playerTransform.GetTranslation(), FRotator( playerTransform.GetRotation() ), params );
		}
		GetGameState<AProjectTapGameState>()->CurrentPawn = ball;
	}
	GetGameState<AProjectTapGameState>()->SetState(AProjectTapGameState::GAME_STATE_PLAYING);
}

ABallPawn* AProjectTapGameMode::getBall()
{
	return ball;
}
