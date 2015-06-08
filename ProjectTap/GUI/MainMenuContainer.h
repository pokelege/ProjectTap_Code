// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "MainMenuContainer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTAP_API AMainMenuContainer : public AActor
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent, BluePrintCallable, Category = MainMenu)
	void PrintMessageProjectTap();
	
	AMainMenuContainer();
};
