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
	FVector hiddenLocation = FVector(810.0f, -2390.0f, 510.0f);
	FVector mainMenuShowLocation = FVector(810.0f, -270.0f, 510.0f);
	FVector optionMenuShowLocation = FVector(800.0f, -530.0f, 290.0f);
	FVector levelSelectMenuShowLocation = FVector(820.0f, -570.0f, 260.0f);
	FVector creditMenuShowLocation = FVector();

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

	void TransitionToMenu(AActor* menuPlaceable, const FVector& pos);
	
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
