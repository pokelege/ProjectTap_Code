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

protected:
	GameState CurrentState = GAME_STATE_PLAYING;
	float cameraSaturation = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	class UProjectTapCameraComponent* CurrentCamera = nullptr;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float aiMaxDistance = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float aiMinDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class ABallPawn* CurrentPawn = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level)
	FName currentLevelToLoadWhenWin;

	DECLARE_MULTICAST_DELEGATE_OneParam( FGameStateChanged , const uint8 )
	UPROPERTY( BlueprintAssignable , Category = "GameState" )
	static FGameStateChanged GameStateChanged;

	DECLARE_MULTICAST_DELEGATE_OneParam( FCameraChanged , UProjectTapCameraComponent* )
	UPROPERTY( BlueprintAssignable , Category = "Camera" )
	FCameraChanged CameraChanged;

	UFUNCTION( BlueprintCallable , Category = GameState )
	void SetGameState(uint8 NewState, bool notifyListeners = true);
	UFUNCTION( BlueprintCallable , Category = Camera)
	void SetCamera(class UProjectTapCameraComponent* camera,  bool notifyListeners = true);
	GameState GetState();
	float getCameraSaturation() const;
	void setCameraSaturation(float value);
};
