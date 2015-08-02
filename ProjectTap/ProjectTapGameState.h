// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameState.h"
#include "CustomGameState.h"
#include "CustomGameMode.h"
#include "ProjectTapGameState.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API AProjectTapGameState : public AGameState
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam( FPlayerChanged , const ABallPawn* )
	UPROPERTY( BlueprintAssignable , Category = "Player" )
		FPlayerChanged PlayerChanged;

	DECLARE_MULTICAST_DELEGATE_OneParam( FGameModeChanged , const CustomGameMode )
	UPROPERTY( BlueprintAssignable , Category = "GameMode" )
		FGameModeChanged GameModeChanged;

	DECLARE_MULTICAST_DELEGATE_OneParam( FGameStateChanged , const CustomGameState )
	UPROPERTY( BlueprintAssignable , Category = "GameState" )
		FGameStateChanged GameStateChanged;

	DECLARE_MULTICAST_DELEGATE_OneParam( FCameraChanged , UProjectTapCameraComponent* )
	UPROPERTY( BlueprintAssignable , Category = "Camera" )
		FCameraChanged CameraChanged;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Level )
		FName currentLevelToLoadWhenWin;
protected:
	UPROPERTY( EditAnywhere , BlueprintReadOnly , Category = Camera )
	class UProjectTapCameraComponent* CurrentCamera = nullptr;
	class AMagnetTile* lastMagnetPull = nullptr;
	class ABallPawn* CurrentPawn = nullptr;
	float cameraSaturation = 1.0f;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Camera )
		float aiMaxDistance = 500.0f;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Camera )
		float aiMinDistance = 100.0f;
protected:
	CustomGameState CurrentState = CustomGameState::GAME_STATE_PLAYING;
	CustomGameMode CurrentMode = CustomGameMode::GAME_MODE_LEVEL;
public:

	UFUNCTION( BlueprintCallable , Category = Player )
	void SetPlayer( ABallPawn* NewPlayer , bool notifyListeners = true );

	UFUNCTION( BlueprintCallable , Category = GameState )
		void SetGameState( CustomGameState NewState , bool notifyListeners = true );
	UFUNCTION( BlueprintCallable , Category = GameMode )
		void SetGameMode( CustomGameMode NewMode , bool notifyListeners = true );
	UFUNCTION( BlueprintCallable , Category = Camera)
	void SetCamera(class UProjectTapCameraComponent* camera,  bool notifyListeners = true);

	AMagnetTile* SetMagnetTile( class AMagnetTile* magnet );
	UFUNCTION( BlueprintCallable , Category = Player )
	ABallPawn* GetPlayer();
	UFUNCTION( BlueprintCallable , Category = GameState )
	CustomGameState GetState();
	UFUNCTION( BlueprintCallable , Category = GameMode )
	CustomGameMode GetMode();
	float getCameraSaturation() const;
	void setCameraSaturation(float value);
};
