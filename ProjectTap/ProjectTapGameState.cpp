// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameState.h"




void AProjectTapGameState::SetState(GameState NewState)
{
	CurrentState = NewState;
}

AProjectTapGameState::GameState AProjectTapGameState::GetState()
{
	return CurrentState;
}