// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameMode.h"
#include "Controllers/MouseController.h"
AProjectTapGameMode::AProjectTapGameMode(const FObjectInitializer& initializer):Super(initializer)
{
  UE_LOG( LogTemp , Warning , TEXT( "mouse" ) );
  PlayerControllerClass = AMouseController::StaticClass();
  
  // set default pawn class to our Blueprinted character
  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/TopDownCharacter"));
  if (PlayerPawnBPClass.Class != NULL)
  {
    DefaultPawnClass = PlayerPawnBPClass.Class;
  }
}