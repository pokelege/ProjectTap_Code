// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "ProjectTapGameState.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTTAP_API AProjectTapGameState : public AGameState
{
	GENERATED_BODY()

public:
	enum GameState {UNKNOWN, GAME_STATE_PLAYING, GAME_STATE_GAME_OVER};
protected:
	GameState CurrentState = UNKNOWN;
public:
	void SetState(GameState NewState);
	GameState GetState();
};