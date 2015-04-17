// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
class ABallPawn;
#include "GameFramework/GameMode.h"
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

  void Respawn();
};
