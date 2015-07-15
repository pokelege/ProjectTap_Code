// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameMode.h"
#include "ProjectTapGameState.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameState.h"
#include "Controllers/MouseController.h"
#include "Pawns/BallPlayerStart.h"
#include "General/ProjectTapCameraComponent.h"
#include "General/ProjectTapCamera.h"

AProjectTapGameMode::AProjectTapGameMode( const FObjectInitializer& initializer ): Super( initializer )
{
	//UE_LOG( LogTemp , Warning , TEXT( "mouse" ) );
	PlayerControllerClass = AMouseController::StaticClass();
	DefaultPawnClass = nullptr;
	GameStateClass = AProjectTapGameState::StaticClass();
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<USoundWave> defaultMusicFile( TEXT( "/Game/Sound/S_DefaultMusic" ) );
	musicPlayer = CreateDefaultSubobject<UAudioComponent>( TEXT( "Music" ) );
	musicPlayer->SetSound( defaultMusicFile.Object );
	musicPlayer->bAutoActivate = false;
	musicPlayer->AttachTo( GetRootComponent() );
	OnGameStateChangedDelegateHandle = AProjectTapGameState::GameStateChanged.AddUFunction( this , TEXT( "OnStateChanged" ) );
}

void AProjectTapGameMode::BeginPlay()
{
	Super::BeginPlay();
	auto gameState = GetGameState<AProjectTapGameState>();
	gameState->CameraChanged.AddUFunction( this , TEXT( "OnCameraChanged" ) );
	if ( UWorld* world = GetWorld() )
	{
		AActor* playerStart = FindPlayerStart( 0, FString( "Player" ) );
		FTransform playerTransform = playerStart->GetTransform();
		if ( ABallPlayerStart* realPlayerStart = Cast<ABallPlayerStart>( playerStart ) )
		{
			auto possibleCamera = Cast<UProjectTapCameraComponent>(realPlayerStart->camera->GetComponentByClass(UProjectTapCameraComponent::StaticClass()));
			FActorSpawnParameters params;
			ball = world->SpawnActor<ABallPawn>(
				ABallPawn::StaticClass(),
				playerTransform.GetTranslation(),
				FRotator( playerTransform.GetRotation()),
				params);

			if (ball != nullptr)
			{
				ball->AddVelocity(realPlayerStart->initialVelocity, realPlayerStart->GetActorLocation());
				if(possibleCamera != nullptr && realPlayerStart->followPlayer)
				{
					ball->setCamera(realPlayerStart);
					possibleCamera = ball->GetCamera();
				}
			}
			gameState->SetCamera( possibleCamera );
			if ( realPlayerStart->music != nullptr )musicPlayer->SetSound( realPlayerStart->music );
		}
		else
		{
			FActorSpawnParameters params;
			ball = world->SpawnActor<ABallPawn>( ABallPawn::StaticClass(), playerTransform.GetTranslation(), FRotator( playerTransform.GetRotation() ), params );
		}

		gameState->CurrentPawn = ball;
	}
	musicPlayer->Play();
	musicPlayer->SetVolumeMultiplier( 0 );
	gameState->SetGameState(GAME_STATE_PLAYING);
}

void AProjectTapGameMode::BeginDestroy()
{
	AProjectTapGameState::GameStateChanged.Remove(OnGameStateChangedDelegateHandle);
	OnGameStateChangedDelegateHandle.Reset();
	Super::BeginDestroy();
}

void AProjectTapGameMode::Respawn()
{
	GetWorld()->GetFirstPlayerController()->ClientTravel( TEXT("?restart"), TRAVEL_MAX );
}

ABallPawn* AProjectTapGameMode::getBall()
{
	return ball;
}

bool AProjectTapGameMode::LoadNextLevel()
{
	if(loadingLevel) return false;
	UGameplayStatics::OpenLevel(GetWorld(),GetGameState<AProjectTapGameState>()->currentLevelToLoadWhenWin);
	return loadingLevel = true;
}
void AProjectTapGameMode::OnStateChanged(const uint8 newState )
{
	if(lastReportedState == newState) return;
	lastReportedState = newState;
	if(camera != nullptr)
	{
		switch(lastReportedState)
		{
			case GAME_STATE_PLAYING:
				camera->FadeIn();
				break;
			case GameState::GAME_STATE_GAME_OVER:
			case GameState::GAME_STATE_WIN:
				camera->FadeOut();
		}
	}
}

void AProjectTapGameMode::OnCameraFaded()
{
	switch(lastReportedState)
	{
		case GameState::GAME_STATE_GAME_OVER:
			Respawn();
			break;
		case GameState::GAME_STATE_WIN:
			LoadNextLevel();
	}
}

void AProjectTapGameMode::OnCameraChanged(UProjectTapCameraComponent* newCamera)
{
	if(camera != nullptr)
	{
		camera->OnFadeIn.Remove( OnCameraFadeInDelegateHandle );
		OnCameraFadeInDelegateHandle.Reset();
		camera->OnFadeOut.Remove( OnCameraFadeOutDelegateHandle );
		OnCameraFadeOutDelegateHandle.Reset();
		camera->OnFadeUpdate.Remove( OnCameraFadeUpdateDelegateHandle );
		OnCameraFadeUpdateDelegateHandle.Reset();
	}
	camera = newCamera;
	if(camera != nullptr)
	{
		OnCameraFadeInDelegateHandle = camera->OnFadeIn.AddUFunction( this , TEXT( "OnCameraFaded" ) );
		OnCameraFadeOutDelegateHandle = camera->OnFadeOut.AddUFunction( this , TEXT( "OnCameraFaded" ) );
		OnCameraFadeUpdateDelegateHandle = camera->OnFadeUpdate.AddUFunction( this , TEXT( "OnCameraFadeUpdate" ) );
	}
}
void AProjectTapGameMode::OnCameraFadeUpdate(const float percent)
{
	if(musicPlayer != nullptr) musicPlayer->SetVolumeMultiplier( percent );
}
