// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MainMenuContainer.h"
#include "../Tiles/Ramp.h"


#define printonscreen(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

AMainMenuContainer::AMainMenuContainer()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UBlueprint> levelSelect(TEXT("/Game/GUI/LevelSelectPlaceable"));
	levelSelectPlacable = Cast<AActor>(levelSelect.Object->GeneratedClass);

	ConstructorHelpers::FObjectFinder<UBlueprint> options(TEXT("/Game/GUI/OptionsPlaceable"));
	auto optionClass = options.Object->GeneratedClass;
}

void AMainMenuContainer::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<AActor> v_itr(GetWorld()); v_itr; ++v_itr)
	{
		auto actorName = v_itr->GetName();
		if (actorName.Equals("OptionsPlaceable"))
		{
			optionsPlacable = *v_itr;
		}
		else if (actorName.Equals("LevelSelectPlaceable"))
		{
			levelSelectPlacable = *v_itr;
		}
		else if (actorName.Equals("CreditsPlaceable"))
		{
			creditsPlacable = *v_itr;
		}
	}

}


void AMainMenuContainer::Tick(float dt)
{
	Super::Tick(dt);
	UpdateMenuTransition(dt);
}

void AMainMenuContainer::UpdateMenuTransition(float dt)
{
	if (bMoveMenu)
	{
		auto moveDir = (menuSlideEndPos - menuSlideStartPos).GetSafeNormal();
		moveDir = bBackToMenu ? -moveDir : moveDir;
		auto goalPos = bBackToMenu ? menuSlideStartPos : menuSlideEndPos;
		auto bReachedGoal = FVector::Dist(currentMenu->GetActorLocation(), goalPos);

		if (bReachedGoal)
		{
			currentMenu->SetActorLocation(goalPos);
			if (bBackToMenu)
			{
				mainMenuPlacable->SetActorLocation(menuSlideEndPos);
			}

			bMoveMenu = false;
			bBackToMenu = false;
		}
		else
		{
			auto moveDelta = transitionSpeed * dt * moveDir;
			currentMenu->SetActorLocation(currentMenu->GetActorLocation() + moveDelta);
			mainMenuPlacable->SetActorLocation(mainMenuPlacable->GetActorLocation() + moveDelta);
		}
	}
}

void AMainMenuContainer::TransitionToMenu(AActor* menuPlaceable)
{
	if (menuPlaceable != currentMenu)
	{
		currentMenu = menuPlaceable;
		auto goalPos = bBackToMenu ? menuSlideStartPos : menuSlideEndPos;
		menuPlaceable->SetActorLocation(goalPos);
	}

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
	currentMenuState = MenuState::CONTINUE;
	//load last played level name
	StartNewGame();
}

void AMainMenuContainer::ToMainMenu()
{
	currentMenuState = MenuState::MAIN_MENU;
	bBackToMenu = true;
	bMoveMenu = true;
}

void AMainMenuContainer::ToOptions()
{
	currentMenuState = MenuState::OPTIONS;
	TransitionToMenu(optionsPlacable);
}

void AMainMenuContainer::ToLevelSelect()
{
	currentMenuState = MenuState::LEVEL_SELECT;
	TransitionToMenu(levelSelectPlacable);
}

void AMainMenuContainer::ToCredits()
{
	currentMenuState = MenuState::CREDITS;
	TransitionToMenu(creditsPlacable);
}

