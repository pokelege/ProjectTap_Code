// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "MainMenuContainer.generated.h"

UENUM(BlueprintType)
enum class MenuState : uint8
{
	MAIN_MENU,
	NEW_GAME,
	CONTINUE,
	LEVEL_SELECT,
	OPTIONS,
	CREDITS,
	QUIT
};

/**
 * 
 */
UCLASS()
class PROJECTTAP_API AMainMenuContainer : public AActor
{
	GENERATED_BODY()
private:

	UUserWidget* levelSelection;
	AActor* currentMenu;

	MenuState currentMenuState = MenuState::MAIN_MENU;
	bool bMoveMenu = false;
	bool bBackToMenu = false;

	void TransitionToMenu(AActor* menuPlaceable);
	void UpdateMenuTransition(float dt);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		FVector menuSlideEndPos = FVector(770.0f, -260.0f, 430.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		FVector menuSlideStartPos = FVector(770.0f, -1070.0f, 430.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		float transitionSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		AActor* mainMenuPlacable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		AActor* levelSelectPlacable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		AActor* optionsPlacable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		AActor* creditsPlacable;

	UFUNCTION(BluePrintCallable, Category = MainMenu)
		void StartNewGame();

	UFUNCTION(BluePrintCallable, Category = MainMenu)
		void ContinueGame();

	UFUNCTION(BluePrintCallable, Category = MainMenu)
		void ToMainMenu();

	UFUNCTION(BluePrintCallable, Category = MainMenu)
		void ToOptions();

	UFUNCTION(BluePrintCallable, Category = MainMenu)
		void ToLevelSelect();

	UFUNCTION(BluePrintCallable, Category = MainMenu)
		void ToCredits();

	virtual void Tick(float dt) override;
	virtual void BeginPlay() override;

	AMainMenuContainer();
};
