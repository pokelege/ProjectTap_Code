// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "MainMenuContainer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTAP_API AMainMenuContainer : public AActor
{
	GENERATED_BODY()
private:

	AActor* currentMenu;

	bool bMoveMenu = false;
	bool bBackToMenu = false;
	bool enabled = true;
	bool bReachedGoal = false;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		AActor* mainMenuPlacable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		AActor* levelSelectPlacable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		AActor* optionsPlacable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		AActor* creditsPlacable;

	UFUNCTION(BluePrintCallable, Category = MainMenu1)
		void StartNewGame();

	void SetWidgetVisibility(AActor* widget, EVisibility visibility);

	void TransitionToMenu(AActor* menuPlaceable);
	
	UFUNCTION(BluePrintCallable, Category = MainMenu3)
		void ContinueGame();

	UFUNCTION(BluePrintCallable, Category = MainMenu4)
		void ToMainMenu();

	UFUNCTION(BluePrintCallable, Category = MainMenu5)
		void ToOptions();

	UFUNCTION(BluePrintCallable, Category = MainMenu6)
		void ToLevelSelect();

	UFUNCTION(BluePrintCallable, Category = MainMenu7)
		void ToCredits();

	virtual void BeginPlay() override;
	virtual void Tick(float dt) override;

	AMainMenuContainer();
};
