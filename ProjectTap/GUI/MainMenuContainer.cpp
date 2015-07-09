// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "../Tiles/Ramp.h"
#include "MainMenuContainer.h"

#define printonscreen(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

float timer = 0.0f;
AMainMenuContainer::AMainMenuContainer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMainMenuContainer::Tick(float dt)
{
	Super::Tick(dt);
}

void AMainMenuContainer::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<AActor> v_itr(GetWorld()); v_itr; ++v_itr)
	{
		auto actorName = v_itr->GetName();
		if (actorName.StartsWith("OptionsPlaceable"))
		{
			optionsPlacable = *v_itr;
		}
		else if (actorName.StartsWith("LevelSelectPlaceable"))
		{
			levelSelectPlacable = *v_itr;
		}
		else if (actorName.StartsWith("CreditsPlaceable"))
		{
			creditsPlacable = *v_itr;
		}
		else if (actorName.StartsWith("MainMenu"))
		{
			mainMenuPlacable = *v_itr;
			currentMenu = *v_itr;
		}
	}

}

void AMainMenuContainer::StartNewGame()
{
	for(TActorIterator<ARamp> v_itr(GetWorld()); v_itr; ++v_itr)
	{
		v_itr->activate();
		break;
	}
	
	SetWidgetVisibility(mainMenuPlacable, EVisibility::Hidden);
}

void AMainMenuContainer::ContinueGame()
{
	//currentMenuState = MenuState::CONTINUE;
	//load last played level name
	StartNewGame();
}

void AMainMenuContainer::ToMainMenu()
{
	//currentMenuState = MenuState::MAIN_MENU;
	TransitionToMenu(mainMenuPlacable);
}

void AMainMenuContainer::ToOptions()
{
	//currentMenuState = MenuState::OPTIONS;
	TransitionToMenu(optionsPlacable);
}

void AMainMenuContainer::ToLevelSelect()
{
	//currentMenuState = MenuState::LEVEL_SELECT;
	TransitionToMenu(levelSelectPlacable);
}

void AMainMenuContainer::ToCredits()
{
	//currentMenuState = MenuState::CREDITS;
	TransitionToMenu(creditsPlacable);
}

void AMainMenuContainer::TransitionToMenu(AActor* menuPlaceable)
{
	if (menuPlaceable != currentMenu)
	{
		SetWidgetVisibility(currentMenu, EVisibility::Collapsed);

		SetWidgetVisibility(menuPlaceable, EVisibility::Visible);

		currentMenu = menuPlaceable;
	}
}

void AMainMenuContainer::SetWidgetVisibility(AActor* widget, EVisibility visibility)
{
	auto component = widget->GetRootComponent();
	auto menuWidget = Cast<UWidgetComponent>(component);
	if (menuWidget != nullptr)
	{
		auto w = menuWidget->GetSlateWidget()->GetChildren()->GetChildAt(0);
		w->SetVisibility(visibility);
	}
}


