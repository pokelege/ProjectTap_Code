// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MainMenuContainer.h"


#define printonscreen(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

AMainMenuContainer::AMainMenuContainer()
{
	PrimaryActorTick.bCanEverTick = false;
}


void AMainMenuContainer::PrintMessageProjectTap_Implementation()
{
	printonscreen(TEXT("click!!!"));
}
