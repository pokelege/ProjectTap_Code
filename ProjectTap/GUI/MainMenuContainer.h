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
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Menu)
		UUserWidget* currentMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		UUserWidget* mainMenuPlacable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		UUserWidget* levelSelectPlacable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		UUserWidget* optionsPlacable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		UUserWidget* creditsPlacable;

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

	AMainMenuContainer();
};
