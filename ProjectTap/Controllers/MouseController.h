// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Tiles/TilesManager.h"
#include "MouseController.generated.h"

/**
 *
 */
UCLASS(config=Game)
class PROJECTTAP_API AMouseController : public APlayerController
{
	GENERATED_BODY()

	float raycastElapseTime = 0.05f;
	float raycastElapseTimeCounter = 0.0f;
	bool raycasted = false;

	UTilesManager btManager;
	bool bCheckForSwipe = false;


	void ActivateOtherTiles(const FHitResult& hit);
	void SendBlockingTile(const FHitResult& hit);
	void SendStrongBlockingTile(const FHitResult& hit);
	void SendGroupedBlockingTile(const FHitResult& hit);
	void SendDraggableMoveTile(const FHitResult& hit);
	void checkObjectHighlight(const FHitResult& hit);

	void NotifyMouseReleased();
	void NotifyMousePressed();
	void DisnableSwipeCheck();
	void EnableSwipeCheck(const FHitResult& hit);

	void RespawnPressed();

	void GetCameraRay(FVector& WorldOrigin, FVector& WorldDirection);

	AActor* currentCamera = nullptr;
public:
  AMouseController(const FObjectInitializer& initializer);
  // Begin PlayerController interface
  virtual void PlayerTick(float DeltaTime) override;
  virtual void SetupInputComponent() override;


};
