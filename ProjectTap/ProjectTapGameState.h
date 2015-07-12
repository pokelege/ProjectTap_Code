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
	enum GameState {UNKNOWN, GAME_STATE_STARTING, GAME_STATE_PLAYING, GAME_STATE_GAME_OVER, GAME_STATE_DYING, GAME_STATE_WIN, GAME_STATE_WINNING};
protected:
	GameState CurrentState = UNKNOWN;
	float cameraSaturation = 1.0f;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float aiMaxDistance = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float aiMinDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class ABallPawn* CurrentPawn = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class AActor* CurrentCamera = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level)
	FName currentLevelToLoadWhenWin;
	void SetState(GameState NewState);
	GameState GetState();
	float getCameraSaturation() const;
	void setCameraSaturation(float value);
};
