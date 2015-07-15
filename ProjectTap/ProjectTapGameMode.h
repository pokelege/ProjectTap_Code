// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameMode.h"
#include "GameState.h"
#include "ProjectTapGameMode.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API AProjectTapGameMode : public AGameMode
{
	GENERATED_BODY()

	class ABallPawn* ball = nullptr;
	FStreamLevelAction* levelStream = nullptr;
	bool loadingLevel = false;
	float time = 0;
	class UProjectTapCameraComponent* camera = nullptr;
	float restartCoolDown = 1.0f;
	int lastReportedState = GameState::UNKNOWN;
	FDelegateHandle OnCameraFadeInDelegateHandle;
	FDelegateHandle OnCameraFadeOutDelegateHandle;
	FDelegateHandle OnCameraFadeUpdateDelegateHandle;
public:

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
	class UAudioComponent* musicPlayer = nullptr;

	AProjectTapGameMode ( const FObjectInitializer& initializer );

	virtual void BeginPlay() override;

	void Respawn();
	class ABallPawn* getBall();
	UFUNCTION()
	bool LoadNextLevel();
	UFUNCTION()
	void OnStateChanged( const uint8 newState );
	UFUNCTION()
	void OnCameraFaded();
	UFUNCTION()
	void OnCameraChanged(UProjectTapCameraComponent* newCamera);
	UFUNCTION()
	void OnCameraFadeUpdate(const float percent);
};
