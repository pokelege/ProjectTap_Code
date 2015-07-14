// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameState.h"
#include "GameState.h"
#include "ProjectTapGameState.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API AProjectTapGameState : public AGameState
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam( AProjectTapGameState , FGameStateChanged , const uint8 )
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

	FGameStateChanged GameStateChanged;

	void SetState(GameState NewState, bool notifyListeners = true);
	GameState GetState();
	float getCameraSaturation() const;
	void setCameraSaturation(float value);
};
