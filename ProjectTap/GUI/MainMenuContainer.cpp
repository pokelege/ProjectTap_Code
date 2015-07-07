// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MainMenuContainer.h"
#include "../Tiles/Ramp.h"


#define printonscreen(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

AMainMenuContainer::AMainMenuContainer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMainMenuContainer::StartNewGame()
{
	for(TActorIterator<ARamp> v_itr(GetWorld()); v_itr; ++v_itr)
	{
		v_itr->activate();
		break;
	}
}

void AMainMenuContainer::ContinueGame()
{
	
}

void AMainMenuContainer::ToMainMenu()
{

}

void AMainMenuContainer::ToOptions()
{

}

void AMainMenuContainer::ToLevelSelect()
{

}

void AMainMenuContainer::ToCredits()
{

}

