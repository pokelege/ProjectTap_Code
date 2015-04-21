// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "../Tiles/TilesManager.h"
#include "ProjectTapGameMode.h"
#include "MouseController.generated.h"

/**
 * 
 */
UCLASS(config=Game)
class PROJECTTAP_API AMouseController : public APlayerController
{
	GENERATED_BODY()

	float raycastElapseTime = 0.1f;
	float raycastElapseTimeCounter = 0.0f;
	bool raycasted = false;

	UTilesManager btManager;
	bool bCheckForSwipe = false;


	void ActivateOtherTiles(const FHitResult& hit);
	void SendBlockingTile(const FHitResult& hit);
	void SendStrongBlockingTile(const FHitResult& hit);
	void SendGroupedBlockingTile(const FHitResult& hit);

	void checkObjectHighlight(const FHitResult& hit);

	void NotifyMouseReleased();
	void NotifyMousePressed();
	void DisnableSwipeCheck();
	void EnableSwipeCheck(const FHitResult& hit);

	void RespawnPressed();


public:
  AMouseController(const FObjectInitializer& initializer);
  // Begin PlayerController interface
  virtual void PlayerTick(float DeltaTime) override;
  virtual void SetupInputComponent() override;

  
};
