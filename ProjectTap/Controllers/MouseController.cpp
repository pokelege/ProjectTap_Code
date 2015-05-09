// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MouseController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "../Tiles/BlockingTile.h"
#include "../Tiles/StrongBlockingTile.h"
#include "../Tiles/GroupedBlockingTile.h"
#include "../Tiles/BaseRampTile.h"
#include "../Tiles/DeflectiveTile.h"
#include "Tiles/MagnetTile.h"



AMouseController::AMouseController(const FObjectInitializer& initializer):Super(initializer)
{
  UE_LOG( LogTemp , Warning , TEXT( "mouse" ) );
  this->bShowMouseCursor = true;
  this->bEnableClickEvents = true;
  this->bEnableTouchEvents = true;
  this->DefaultMouseCursor = EMouseCursor::Default;

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


void AMouseController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	FHitResult hit;

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
		SendGroupedBlockingTile(hit);
		checkObjectHighlight(hit);
	}

	btManager.Tick(DeltaTime);

	auto s = GetWorld();
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
	SendStrongBlockingTile(hit);
	SendBlockingTile(hit);
	SendGroupedBlockingTile(hit);
	EnableSwipeCheck(hit);

}


void AMouseController::ActivateOtherTiles(const FHitResult& hit)
{
	bool found = false;

	auto ramp = Cast<ABaseRampTile>(hit.Actor.Get());
	if (ramp != nullptr)
	{
		ramp->activate();
		found = true;
	}

	if (!found)
	{
		auto mag = Cast<AMagnetTile>(hit.Actor.Get());
		if (mag != nullptr)
		{
			mag->activate();
			found = true;
		}
	}

	if (!found)
	{
		auto def = Cast<ADeflectiveTile>(hit.Actor.Get());
		if (def != nullptr)
		{
			def->activate();
			found = true;
		}
	}

}


void AMouseController::EnableSwipeCheck(const FHitResult& hit)
{

	auto gbt = Cast<AGroupedBlockingTile>(hit.Actor.Get());
	if (gbt != nullptr)
	{
		btManager.SetEnableSwipeCheck(true);
		bCheckForSwipe = true;
	}
}

void AMouseController::DisnableSwipeCheck()
{
	btManager.SetEnableSwipeCheck(false);
	bCheckForSwipe = false;
}


void AMouseController::SendBlockingTile(const FHitResult& hit)
{

	auto bt = Cast<ABlockingTile>(hit.Actor.Get());
	auto type = hit.Actor.Get();
	bool canContinue = true;

	if (bt != nullptr)
	{
		canContinue = false;
		btManager.AddTile(bt);
	}
}

void AMouseController::SendStrongBlockingTile(const FHitResult& hit)
{
	auto sbt = Cast<AStrongBlockingTile>(hit.Actor.Get());
	if (sbt != nullptr)
	{
		btManager.AddTile(sbt);
	}
}


void AMouseController::SendGroupedBlockingTile(const FHitResult& hit)
{
	auto gbt = Cast<AGroupedBlockingTile>(hit.Actor.Get());
	if (gbt != nullptr)
	{
		btManager.SetEnableSwipeCheck(true);
		btManager.AddTile(gbt);
	}
}

void AMouseController::NotifyMouseReleased()
{
	btManager.DeactivateStrongBlockingTile();
	btManager.SetEnableSwipeCheck(false);

}

void AMouseController::RespawnPressed()
{
	AProjectTapGameMode* gameMode = Cast<AProjectTapGameMode>(GetWorld()->GetAuthGameMode());
	ABallPawn* ball = gameMode->getBall();
	if(ball != nullptr) ball->Kill();
}