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
	class AActor* camera = nullptr;
	float restartCoolDown = 1.0f;
	int lastReportedState = GameState::UNKNOWN;
public:

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
	class UAudioComponent* musicPlayer = nullptr;

	AProjectTapGameMode ( const FObjectInitializer& initializer );

	virtual void BeginPlay() override;

	virtual void Tick ( float DeltaTime ) override;

	void Respawn();
	class ABallPawn* getBall();
	UFUNCTION()
	bool LoadNextLevel();
	UFUNCTION()
	void OnStateChanged( const uint8 newState );
};
