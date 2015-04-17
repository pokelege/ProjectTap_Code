// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "ProjectTapGameState.generated.h"


#define GAME_STATE_PLAYING FName("Playing", 0)
#define GAME_STATE_GAME_OVER FName("GameOver", 0)
/**
 * 
 */
UCLASS()
class PROJECTTAP_API AProjectTapGameState : public AGameState
{
	GENERATED_BODY()

};
