// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MainMenuContainer.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API AMainMenuContainer : public AActor
{
	GENERATED_BODY()
		FVector hiddenLocation = FVector( 810.0f , -2390.0f , 510.0f );
	FVector mainMenuShowLocation = FVector( 810.0f , -270.0f , 510.0f );
	FVector optionMenuShowLocation = FVector( 800.0f , -530.0f , 290.0f );
	FVector levelSelectMenuShowLocation = FVector( 820.0f , -530.0f , 250.0f );
	FVector creditMenuShowLocation = FVector();
	AActor* currentMenu = nullptr;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Menu )
		AActor* mainMenuPlacable = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Menu )
		AActor* levelSelectPlacable = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Menu )
		AActor* optionsPlacable = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Menu )
		AActor* creditsPlacable = nullptr;
private:

	bool bMoveMenu = false;
	bool bBackToMenu = false;
	bool enabled = true;
	bool bReachedGoal = false;

public:

	UFUNCTION( BluePrintCallable , Category = MainMenu1 )
		void StartNewGame();

	void SetWidgetVisibility( AActor* widget , EVisibility visibility );

	void TransitionToMenu( AActor* menuPlaceable , const FVector& pos );

	UFUNCTION( BluePrintCallable , Category = MainMenu )
		void ContinueGame();

	UFUNCTION( BluePrintCallable , Category = MainMenu )
		void ToMainMenu();

	UFUNCTION( BluePrintCallable , Category = MainMenu )
		void ToOptions();

	UFUNCTION( BluePrintCallable , Category = MainMenu )
		void ToLevelSelect();

	UFUNCTION( BluePrintCallable , Category = MainMenu )
		void ToCredits();

	UFUNCTION( BluePrintCallable , Category = MainMenu )
		void InitializeOptionsMenu( UButton* btnLow ,
		UButton* btnMid ,
		UButton* btnHigh ,
		UButton* btnUltra ,
		UButton* graphicsDescribButton ,
		UButton*& prevGraphicsButton ,
		FString& graphicsQualityString ,
		UButton* resolution ,
		TArray<FString> resolutionStrings ,
		int32& currResolutionIndex ,
		UCheckBox* fullScreenBox ,
		USlider* volumeSlider );

	UFUNCTION( BluePrintCallable , Category = MainMenu )
		void ChangeSettings( const FString& graphicsSetting ,
		const FString& resolution ,
		bool fullScreen );

	UFUNCTION( BluePrintCallable , Category = MainMenu )
		void SetSoundVolume( float slider );


	virtual void BeginPlay() override;
	virtual void Tick( float dt ) override;

	AMainMenuContainer();
private:
	FIntPoint getResolutionByString( const FString& screen );
	FString getCurrentResolutionString();
	void SetGraphicsScalability( const FString& quality ,
								 UGameUserSettings* settings );
	void SetGraphicsLow( UGameUserSettings* settings );
	void SetGraphicsMid( UGameUserSettings* settings );
	void SetGraphicsHigh( UGameUserSettings* settings );
	void SetGraphicsUltra( UGameUserSettings* settings );
	void SetGraphcisScalabilityNumber( UGameUserSettings* settings , int32 quality );

	void InitGraphicsWidgets( UButton* btnLow ,
							  UButton* btnMid ,
							  UButton* btnHigh ,
							  UButton* btnUltra ,
							  UButton* describButton ,
							  UButton*& prevGraphicsButton );

	void InitResolutionWidgets( UButton* resolutionButton ,
								int32& currResolutionIndex ,
								TArray<FString>& resolutionStrings );

	void InitFullScreenWidget( UCheckBox* checkBox );

	void InitSoundWidget( USlider* slider );

	int32 GetIndexByResolutionString( TArray<FString>& resolutionStrings ,
									  const FString& resolution );
};
