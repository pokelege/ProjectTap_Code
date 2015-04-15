// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "../Tiles/TilesManager.h"
#include "MouseController.generated.h"

/**
 * 
 */
UCLASS(config=Game)
class PROJECTTAP_API AMouseController : public APlayerController
{
	GENERATED_BODY()

	UTilesManager* btManager;
	float swipeElapseTime = 0.1f;
	float swipeElapseTimeCounter = 0.0f;
	bool bCheckForSwipe = false;

public:
  AMouseController(const FObjectInitializer& initializer);
  // Begin PlayerController interface
  virtual void PlayerTick(float DeltaTime) override;
  virtual void SetupInputComponent() override;

  void ActivateOtherTiles();
  void SendBlockingTile();
  void SendStrongBlockingTile();
  void SendGroupedBlockingTile();

  void NotifyMouseReleased();
  void DisnableSwipeCheck();
  void EnableSwipeCheck();
};
