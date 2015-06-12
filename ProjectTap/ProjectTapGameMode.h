// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameMode.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameMode.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API AProjectTapGameMode : public AGameMode
{
	GENERATED_BODY()

	ABallPawn* ball;

	FStreamLevelAction* levelStream;
	bool loadingLevel = false;
	float time = 0;
	class AActor* camera;
	float restartCoolDown = 1.0f;
public:

	AProjectTapGameMode ( const FObjectInitializer& initializer );

	virtual void BeginPlay() override;

	virtual void Tick ( float DeltaTime ) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Respawn();

	ABallPawn* getBall();
	UFUNCTION()
	bool LoadNextLevel();
};
