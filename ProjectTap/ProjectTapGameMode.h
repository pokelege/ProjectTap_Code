// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameMode.h"
#include "CustomGameState.h"
#include "ProjectTapGameMode.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API AProjectTapGameMode : public AGameMode
{
	GENERATED_BODY()

	FDelegateHandle OnCameraFadeInDelegateHandle;
	FDelegateHandle OnCameraFadeOutDelegateHandle;
	FDelegateHandle OnCameraFadeUpdateDelegateHandle;
	FDelegateHandle OnGameStateChangedDelegateHandle;
	FDelegateHandle OnCameraChangedDelegateHandle;
	FStreamLevelAction* levelStream = nullptr;
	class UProjectTapCameraComponent* camera = nullptr;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
	class UAudioComponent* musicPlayer = nullptr;
private:
	float time = 0;
	float restartCoolDown = 1.0f;
	CustomGameState lastReportedState = CustomGameState::GAME_STATE_UNKNOWN;
	bool isMenu = false;
	bool loadingLevel = false;
public:
	AProjectTapGameMode ( const FObjectInitializer& initializer );
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	void Respawn();
	UFUNCTION()
	bool LoadNextLevel();
	UFUNCTION()
		void OnStateChanged( const CustomGameState newState );
	UFUNCTION()
	void OnCameraFaded();
	UFUNCTION()
	void OnCameraChanged(UProjectTapCameraComponent* newCamera);
	UFUNCTION()
	void OnCameraFadeUpdate(const float percent);
	virtual void StartPlay() override;
};
