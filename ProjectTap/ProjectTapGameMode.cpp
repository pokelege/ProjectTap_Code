// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameMode.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameState.h"
#include "Controllers/MouseController.h"
#include "Pawns/BallPlayerStart.h"

AProjectTapGameMode::AProjectTapGameMode( const FObjectInitializer& initializer ): Super( initializer )
{
	//UE_LOG( LogTemp , Warning , TEXT( "mouse" ) );
	PlayerControllerClass = AMouseController::StaticClass();
	DefaultPawnClass = 0;
	GameStateClass = AProjectTapGameState::StaticClass();
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USoundWave> defaultMusicFile( TEXT( "/Game/Sound/S_DefaultMusic" ) );
	musicPlayer = CreateDefaultSubobject<UAudioComponent>( TEXT( "Music" ) );
	musicPlayer->SetSound( defaultMusicFile.Object );
	musicPlayer->bAutoActivate = false;
	musicPlayer->AttachTo( GetRootComponent() );
}

void AProjectTapGameMode::BeginPlay()
{
	if ( UWorld* world = GetWorld() )
	{
		AActor* playerStart = FindPlayerStart( 0, FString( "Player" ) );
		FTransform playerTransform = playerStart->GetTransform();
		if ( ABallPlayerStart* realPlayerStart = Cast<ABallPlayerStart>( playerStart ) )
		{
			camera = realPlayerStart->camera;
			FActorSpawnParameters params;
			//AActor* spawned = world->SpawnActor(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotation(playerStart.GetRotation());
			ball = world->SpawnActor<ABallPawn>(
				ABallPawn::StaticClass(),
				playerTransform.GetTranslation(),
				FRotator( playerTransform.GetRotation()),
				params);

			if (ball != nullptr)
			{
				ball->AddVelocity(realPlayerStart->initialVelocity, realPlayerStart->GetActorLocation());
				if(camera != nullptr && realPlayerStart->followPlayer)
				{
					ball->setCamera(realPlayerStart);
					camera = ball->getCamera();
				}
			}
			if ( realPlayerStart->music != nullptr )musicPlayer->SetSound( realPlayerStart->music );
		}
		else
		{
			FActorSpawnParameters params;
			ball = world->SpawnActor<ABallPawn>( ABallPawn::StaticClass(), playerTransform.GetTranslation(), FRotator( playerTransform.GetRotation() ), params );
		}

		GetGameState<AProjectTapGameState>()->CurrentPawn = ball;
	}
	GetGameState<AProjectTapGameState>()->CurrentCamera = camera;
	GetGameState<AProjectTapGameState>()->SetState(AProjectTapGameState::GAME_STATE_STARTING);
	musicPlayer->Play();
	musicPlayer->SetVolumeMultiplier( 0 );
}

void AProjectTapGameMode::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if ( GetGameState<AProjectTapGameState>()->GetState() == AProjectTapGameState::GAME_STATE_STARTING && GetGameState<AProjectTapGameState>()->CurrentCamera != nullptr )
	{
		time += DeltaTime;
		auto cameraToChangeTest = GetGameState<AProjectTapGameState>()->CurrentCamera->GetComponentByClass( UCameraComponent::StaticClass() );
		auto cameraToChange = Cast<UCameraComponent>( cameraToChangeTest );
		if ( cameraToChange )
		{
			float fadeValue = FMath::Clamp<float>( time / restartCoolDown , 0 , 1 );
			cameraToChange->PostProcessSettings.bOverride_ColorGain = true;
			cameraToChange->PostProcessSettings.ColorGain = FVector( fadeValue , fadeValue , fadeValue );
			musicPlayer->SetVolumeMultiplier( fadeValue );
		}
		if ( time >= restartCoolDown )
		{
			time = 0;
			GetGameState<AProjectTapGameState>()->SetState( AProjectTapGameState::GAME_STATE_PLAYING );
		}
	}
	else if (GetGameState<AProjectTapGameState>()->GetState() == AProjectTapGameState::GAME_STATE_GAME_OVER )
	{
		time += DeltaTime;
		auto cameraToChangeTest = GetGameState<AProjectTapGameState>()->CurrentCamera->GetComponentByClass( UCameraComponent::StaticClass() );
		auto cameraToChange = Cast<UCameraComponent>( cameraToChangeTest );
		if ( cameraToChange )
		{
			float fadeValue = 1 - FMath::Clamp<float>( time / restartCoolDown , 0 , 1 );
			cameraToChange->PostProcessSettings.bOverride_ColorGain = true;
			cameraToChange->PostProcessSettings.ColorGain = FVector( fadeValue , fadeValue , fadeValue );
			musicPlayer->SetVolumeMultiplier( fadeValue );
		}
		if(time >= restartCoolDown)
		{
			Respawn();
		}
	}
	else if (GetGameState<AProjectTapGameState>()->GetState() == AProjectTapGameState::GAME_STATE_WIN)
	{
		//printonscreen( "You win!" );
		time += DeltaTime;
		auto cameraToChangeTest = GetGameState<AProjectTapGameState>()->CurrentCamera->GetComponentByClass( UCameraComponent::StaticClass() );
		auto cameraToChange = Cast<UCameraComponent>( cameraToChangeTest );
		if ( cameraToChange )
		{
			float fadeValue = 1 - FMath::Clamp<float>( time / restartCoolDown , 0 , 1 );
			cameraToChange->PostProcessSettings.bOverride_ColorGain = true;
			cameraToChange->PostProcessSettings.ColorGain = FVector( fadeValue , fadeValue , fadeValue );
			musicPlayer->SetVolumeMultiplier( fadeValue );
		}
		if(time >= restartCoolDown)
		{
			LoadNextLevel();
		}
	}
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
