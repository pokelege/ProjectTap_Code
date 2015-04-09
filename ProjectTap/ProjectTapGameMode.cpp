// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameMode.h"
#include "Controllers/MouseController.h"
AProjectTapGameMode::AProjectTapGameMode(const FObjectInitializer& initializer):Super(initializer)
{
  UE_LOG( LogTemp , Warning , TEXT( "mouse" ) );
  PlayerControllerClass = AMouseController::StaticClass();
}