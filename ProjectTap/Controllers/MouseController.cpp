// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MouseController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "../Tiles/BlockingTile.h"
#include "../Tiles/StrongBlockingTile.h"


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

	InputComponent->BindAction("ActivateCube", IE_Released, this, &AMouseController::SendBlockingTile);
	InputComponent->BindAction("ActivateCube", IE_Pressed, this, &AMouseController::SendStrongBlockingTile);
	InputComponent->BindAction("ActivateCube", IE_Released, this, &AMouseController::NotifyMouseReleased);
}

void AMouseController::SendBlockingTile()
{
	FHitResult hit;
	GetHitResultUnderCursor(ECC_Visibility, false, hit);

	if (hit.bBlockingHit)
	{
		auto bt = Cast<ABlockingTile>(hit.Actor.Get());
		bool canContinue = true;

		if (bt != nullptr)
		{
			canContinue = false;
			btManager.AddTile(bt);
		}
		
	}
}

void AMouseController::SendStrongBlockingTile()
{
	FHitResult hit;
	GetHitResultUnderCursor(ECC_Visibility, false, hit);


	auto sbt = Cast<AStrongBlockingTile>(hit.Actor.Get());
	if (sbt != nullptr)
	{
		btManager.AddTile(sbt);
	}

}

void AMouseController::NotifyMouseReleased()
{
	btManager.DeactivateStrongBlockingTile();
}

