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

	void LoadNextLevel();
	float time = 0;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameModeSettings)
	float restartCoolDown = 1.0f;

	AProjectTapGameMode ( const FObjectInitializer& initializer );

	virtual void BeginPlay() override;

	virtual void Tick ( float DeltaTime ) override;

	void Respawn();

	ABallPawn* getBall();
};
