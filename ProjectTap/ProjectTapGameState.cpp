// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameState.h"

float AProjectTapGameState::getCameraSaturation() const
{
	return cameraSaturation;
}

void AProjectTapGameState::setCameraSaturation(float value)
{
	cameraSaturation = value;
}
void AProjectTapGameState::SetGameState( CustomGameState NewState , bool notifyListeners )
{
	CurrentState = NewState;
	if ( notifyListeners ) GameStateChanged.Broadcast( CurrentState );
}

void AProjectTapGameState::SetCamera(UProjectTapCameraComponent* camera, bool notifyListeners)
{
	CurrentCamera = camera;
	if(notifyListeners) CameraChanged.Broadcast(CurrentCamera);
}

CustomGameState AProjectTapGameState::GetState()
{
	return CurrentState;
}
void AProjectTapGameState::SetGameMode( CustomGameMode NewMode , bool notifyListeners )
{
	CurrentMode = NewMode;
	if ( notifyListeners ) GameModeChanged.Broadcast( CurrentMode );
}

CustomGameMode AProjectTapGameState::GetMode()
{
	return CurrentMode;
}

class AMagnetTile* AProjectTapGameState::SetMagnetTile( class AMagnetTile* magnet )
{
	AMagnetTile* lastMagnet = lastMagnetPull;
	lastMagnetPull = magnet;
	return lastMagnet;
}

ABallPawn* AProjectTapGameState::GetPlayer()
{
	return CurrentPawn;
}

void AProjectTapGameState::SetPlayer( ABallPawn* NewPlayer , bool notifyListeners )
{
	CurrentPawn = NewPlayer;
	if ( notifyListeners ) PlayerChanged.Broadcast( CurrentPawn );
}