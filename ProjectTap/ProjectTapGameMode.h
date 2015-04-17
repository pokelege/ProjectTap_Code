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

public:
  AProjectTapGameMode(const FObjectInitializer& initializer);

  virtual void BeginPlay() override;

  virtual void Tick(float DeltaTime) override;

  void Respawn();
};
