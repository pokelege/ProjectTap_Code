// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameState.h"


AProjectTapGameState::FGameStateChanged AProjectTapGameState::GameStateChanged = FGameStateChanged();

float AProjectTapGameState::getCameraSaturation() const
{
	return cameraSaturation;
}

void AProjectTapGameState::setCameraSaturation(float value)
{
	cameraSaturation = value;
}
void AProjectTapGameState::SetGameState( uint8 NewState , bool notifyListeners )
{
	CurrentState = (GameState)NewState;
	if ( notifyListeners ) GameStateChanged.Broadcast( CurrentState );
}

void AProjectTapGameState::SetCamera(UProjectTapCameraComponent* camera, bool notifyListeners)
{
	CurrentCamera = camera;
	if(notifyListeners) CameraChanged.Broadcast(CurrentCamera);
}

GameState AProjectTapGameState::GetState()
{
	return CurrentState;
}
