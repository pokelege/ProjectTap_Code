// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MouseController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "BlockingTileManager.h"

AMouseController::AMouseController(const FObjectInitializer& initializer):Super(initializer)
{
  UE_LOG( LogTemp , Warning , TEXT( "mouse" ) );
  this->bShowMouseCursor = true;
  this->bEnableClickEvents = true;
  this->bEnableTouchEvents = true;
  this->DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AMouseController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AMouseController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("ActivateCube", IE_Released, this, &AMouseController::ActivateCube);
}

void AMouseController::ActivateCube()
{
	FHitResult hit;
	GetHitResultUnderCursor(ECC_Visibility, false, hit);

	if (hit.bBlockingHit)
	{
		auto block = dynamic_cast<ABlockingTile*>(hit.Actor.Get());

		if (block != nullptr)
		{
			BlockingTileManager::AddTile(block);
		}
	}
}