// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MouseController.h"

AMouseController::AMouseController(const FObjectInitializer& initializer):Super(initializer)
{
  UE_LOG( LogTemp , Warning , TEXT( "mouse" ) );
  this->bShowMouseCursor = true;
  this->bEnableClickEvents = true;
  this->bEnableTouchEvents = true;
}
