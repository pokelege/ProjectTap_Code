// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MouseController.h"
#include "ProjectTapGameState.h"
#include "Tiles/Tile.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameMode.h"
#include "General/ProjectTapCameraComponent.h"
#include "ProjectTapGameState.h"
#include "Tiles/BlockingTile.h"
#include "Tiles/StrongBlockingTile.h"
#include "Tiles/GroupedBlockingTile.h"
#include "Tiles/DraggableMoveTile.h"

AMouseController::AMouseController(const FObjectInitializer& initializer):Super(initializer)
{
  UE_LOG( LogTemp , Warning , TEXT( "mouse" ) );
  this->bShowMouseCursor = true;
  this->bEnableClickEvents = true;
  this->bEnableTouchEvents = true;
  this->DefaultMouseCursor = EMouseCursor::Default;
}

void AMouseController::BeginPlay()
{
	GetWorld()->GetGameState<AProjectTapGameState>()->CameraChanged.AddUFunction( this , TEXT( "OnCameraChanged" ) );
}


void AMouseController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
	InputComponent->BindAction("Respawn", IE_Pressed, this, &AMouseController::RespawnPressed);
	InputComponent->BindAction("ActivateCube", IE_Pressed, this, &AMouseController::NotifyMousePressed);
	InputComponent->BindAction("ActivateCube", IE_Released, this, &AMouseController::NotifyMouseReleased);
	InputComponent->BindAction("ActivateCube", IE_Released, this, &AMouseController::DisnableSwipeCheck);

}

void AMouseController::OnCameraChanged(UProjectTapCameraComponent* newCamera)
{
	currentCamera = newCamera;
	if(newCamera != nullptr)
	{
		SetViewTarget(currentCamera->GetAttachmentRootActor());
	}
}

void AMouseController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	FHitResult hit;
	auto gameMode = Cast<AProjectTapGameMode>(GetWorld()->GetAuthGameMode());

	//cast a ray for every 1/10 of a second
	if (raycastElapseTimeCounter < raycastElapseTime)
	{
		raycastElapseTimeCounter += DeltaTime;
		raycasted = false;
	}
	else
	{
		raycastElapseTimeCounter = 0.0f;
		raycasted = true;
		GetHitResultUnderCursor(ECC_Visibility, false, hit);
	}

	if (raycasted)
	{
		FVector origin;
		FVector direction;
		GetCameraRay(origin, direction);
		btManager.SetCameraRay(hit, origin, direction);
		checkObjectHighlight(hit);
	}

	if (bCheckForSwipe)
	{
		SendGroupedBlockingTile(hit);
	}

	btManager.Tick(DeltaTime);
}

void AMouseController::checkObjectHighlight(const FHitResult& hit)
{
	auto tile = Cast<ATile>(hit.Actor.Get());

	btManager.HighLightTile(tile);
}


void AMouseController::NotifyMousePressed()
{
	FHitResult hit;
	GetHitResultUnderCursor(ECC_Visibility, false, hit);
	ActivateOtherTiles(hit);
	EnableSwipeCheck(hit);
}


void AMouseController::ActivateOtherTiles(const FHitResult& hit)
{
	bool found = false;

	auto DraggableMoveTile = Cast<ADraggableMoveTile>(hit.Actor.Get());
	if (DraggableMoveTile != nullptr)
	{
		btManager.AddTile(DraggableMoveTile);
		found = true;
	}

	if (!found)
	{
		auto GroupedBlockingTile = Cast<AGroupedBlockingTile>(hit.Actor.Get());
		if (GroupedBlockingTile != nullptr && GroupedBlockingTile->IsEnabled())
		{
			btManager.AddTile(GroupedBlockingTile);
			found = true;
		}
	}

	if (!found)
	{
		auto StrongBlockingTile = Cast<AStrongBlockingTile>(hit.Actor.Get());
		if (StrongBlockingTile != nullptr && StrongBlockingTile->IsEnabled())
		{
			btManager.AddTile(StrongBlockingTile);
			found = true;
		}
	}

	if (!found)
	{
		auto BlockingTile = Cast<ABlockingTile>(hit.Actor.Get());
		if (BlockingTile != nullptr && BlockingTile->IsEnabled())
		{
			btManager.AddTile(BlockingTile);
			found = true;
		}
	}

	if (!found)
	{
		auto tile = Cast<ATile>(hit.Actor.Get());
		if (tile != nullptr && tile->IsEnabled())
		{
			tile->activate();
			found = true;
		}
	}

}


void AMouseController::EnableSwipeCheck(const FHitResult& hit)
{
	if (!bCheckForSwipe)
	{
		auto gbt = Cast<AGroupedBlockingTile>(hit.Actor.Get());
		auto draggableMoveTile = Cast<ADraggableMoveTile>(hit.Actor.Get());
		if (gbt != nullptr || draggableMoveTile != nullptr)
		{
			btManager.SetEnableSwipeCheck(true);
			bCheckForSwipe = true;
		}
	}
}


void AMouseController::DisnableSwipeCheck()
{
	if (bCheckForSwipe)
	{
		btManager.SetEnableSwipeCheck(false);
		bCheckForSwipe = false;
	}
}

void AMouseController::SendDraggableMoveTile(const FHitResult& hit)
{
	auto dm = Cast<ADraggableMoveTile>(hit.Actor.Get());
	if (dm != nullptr)
	{
		btManager.AddTile(dm);
	}
}


void AMouseController::SendBlockingTile(const FHitResult& hit)
{

	auto type = hit.Actor.Get();
	auto bt = Cast<ABlockingTile>(type);
	bool canContinue = true;

	if (bt != nullptr && bt->IsEnabled())
	{
		canContinue = false;
		btManager.AddTile(bt);
	}
}

void AMouseController::SendStrongBlockingTile(const FHitResult& hit)
{
	auto sbt = Cast<AStrongBlockingTile>(hit.Actor.Get());
	if (sbt != nullptr && sbt->IsEnabled())
	{
		btManager.AddTile(sbt);
	}
}


void AMouseController::SendGroupedBlockingTile(const FHitResult& hit)
{
	auto gbt = Cast<AGroupedBlockingTile>(hit.Actor.Get());
	if (gbt != nullptr && gbt->IsEnabled())
	{
		btManager.SetEnableSwipeCheck(true);
		btManager.AddTile(gbt);
	}
}

void AMouseController::NotifyMouseReleased()
{
	btManager.MouseRelease();

}

void AMouseController::RespawnPressed()
{
	AProjectTapGameState* gamestate = Cast<AProjectTapGameState>( GetWorld()->GetGameState() );
	auto currentState = gamestate->GetState();
	ABallPawn* ball = gamestate->GetPlayer();
	if (ball != nullptr) ball->Kill();
}


void AMouseController::GetCameraRay(FVector& WorldOrigin, FVector& WorldDirection)
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	FVector2D MousePosition;
	if (LocalPlayer)
	{
		if (!LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
		{
			return;
		}
	}

	// Early out if we clicked on a HUD hitbox
	if (GetHUD() != NULL && GetHUD()->GetHitBoxAtCoordinates(MousePosition, true))
	{
		return;
	}

	if (LocalPlayer != NULL && LocalPlayer->ViewportClient != NULL && LocalPlayer->ViewportClient->Viewport != NULL)
	{
		// Create a view family for the game viewport
		FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
			LocalPlayer->ViewportClient->Viewport,
			GetWorld()->Scene,
			LocalPlayer->ViewportClient->EngineShowFlags)
			.SetRealtimeUpdate(true));


		// Calculate a view where the player is to update the streaming from the players start location
		FVector ViewLocation;
		FRotator ViewRotation;
		FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, /*out*/ ViewLocation, /*out*/ ViewRotation, LocalPlayer->ViewportClient->Viewport);

		if (SceneView)
		{

			SceneView->DeprojectFVector2D(MousePosition, WorldOrigin, WorldDirection);
		}
	}
}
