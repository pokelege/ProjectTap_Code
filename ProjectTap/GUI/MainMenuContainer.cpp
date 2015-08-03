// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MainMenuContainer.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Slider.h"
#include "Runtime/UMG/Public/Components/CheckBox.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/Engine/Public/AudioDevice.h"
#include "Tiles/Ramp.h"
#include "Tiles/EndTile.h"

AMainMenuContainer::AMainMenuContainer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMainMenuContainer::InitializeOptionsMenu(
	UButton* btnLow,
	UButton* btnMid,
	UButton* btnHigh,
	UButton* btnUltra,
	UButton* graphicsDescribButton,
	UButton*& prevGraphicsButton,
	FString& graphicsQualityString,
	UButton* resolution,
	TArray<FString> resolutionStrings,
	int32& currResolutionIndex,
	UCheckBox* fullScreenBox,
	USlider* volumeSlider)
{					
	InitGraphicsWidgets(btnLow, btnMid, btnHigh, btnUltra, graphicsDescribButton, prevGraphicsButton);
	InitResolutionWidgets(resolution, currResolutionIndex, resolutionStrings);
	InitFullScreenWidget(fullScreenBox);
	InitSoundWidget(volumeSlider);
}


void AMainMenuContainer::InitGraphicsWidgets(UButton* btnLow,
	UButton* btnMid,
	UButton* btnHigh,
	UButton* btnUltra,
	UButton* describButton,
	UButton*& prevGraphicsButton)
{
	auto qualityLevel = GEngine->GameUserSettings->ScalabilityQuality.EffectsQuality;

	auto* textBlock = Cast<UTextBlock>(describButton->GetChildAt(0));
	FText text;

	if (qualityLevel == 0)
	{
		textBlock->SetText(FText::FromString("lowest graphics setting. Good for computers with lower performance"));
		btnLow->SetBackgroundColor(FLinearColor(1.0f, .5f, .0f));
		prevGraphicsButton = btnLow;
	}
	else if (qualityLevel == 1)
	{
		textBlock->SetText(FText::FromString("Medium graphics settings. Enabled some visual effects."));
		btnMid->SetBackgroundColor(FLinearColor(1.0f, .5f, .0f));
		prevGraphicsButton = btnMid;
	}
	else if (qualityLevel == 2)
	{
		textBlock->SetText(FText::FromString("The game will look beautiful. Requires a good graphics card."));
		btnHigh->SetBackgroundColor(FLinearColor(1.0f, .5f, .0f));
		prevGraphicsButton = btnHigh;
	}
	else if (qualityLevel == 3)
	{
		textBlock->SetText(FText::FromString("Ultimate graphics. All visual effects enabled."));
		btnUltra->SetBackgroundColor(FLinearColor(1.0f, .5f, .0f));
		prevGraphicsButton = btnUltra;
	}
}

void AMainMenuContainer::InitResolutionWidgets(UButton* resolutionButton,
	int32& currResolutionIndex,
	TArray<FString>& resolutionStrings)

{
	auto textBlock = Cast<UTextBlock>(resolutionButton->GetChildAt(0));
	auto currResolutionString = getCurrentResolutionString();
	
	for (int32 i = 0; i < resolutionStrings.Num(); i++)
	{
		auto& str = resolutionStrings[i];
		if (str.Equals(currResolutionString))
		{
			currResolutionIndex = i;
			textBlock->SetText(FText::FromString(currResolutionString));
			break;
		}
	}
}

void AMainMenuContainer::InitFullScreenWidget(UCheckBox* checkBox)
{
	//auto mode = GEngine->GameUserSettings->GetFullscreenMode();
	//auto fullScreen = mode == EWindowMode::Fullscreen;
	//checkBox->SetIsChecked(fullScreen);
}

void AMainMenuContainer::InitSoundWidget(USlider* slider)
{
	auto audioDevice = GEngine->GetActiveAudioDevice();
	auto soundClasses = audioDevice->SoundClasses;

	for (auto& sc : soundClasses)
	{
		auto key = sc.Key;
		if (key != nullptr && key->GetName().Equals("Master"))
		{
			auto volume = audioDevice->GetSoundClassCurrentProperties(key)->Volume;
			slider->SetValue(volume);
			break;
		}
	}

}

void AMainMenuContainer::SetSoundVolume(float slider)
{
	auto audioDevice = GEngine->GetActiveAudioDevice();
	auto soundClasses = audioDevice->SoundClasses;

	for (auto& sc : soundClasses)
	{
		auto key = sc.Key;
		if (key != nullptr && key->GetName().Equals("Master"))
		{
			key->Properties.Volume = slider;
			break;
		}
	}
}


int32 AMainMenuContainer::GetIndexByResolutionString(TArray<FString>& resolutionStrings,
													const FString& resolution)
{
	auto index = -1;
	for (int32 i = 0; i < resolutionStrings.Num(); i++)
	{
		if (resolutionStrings[i].Equals(resolution))
		{
			index = i;
			break;
		}
	}
	return index;
}

void AMainMenuContainer::Tick(float dt)
{
	Super::Tick(dt);
}

void AMainMenuContainer::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogWindows, Warning, TEXT("init menu container"));
	for (TActorIterator<AActor> v_itr(GetWorld()); v_itr; ++v_itr)
	{
		auto actorName = v_itr->GetName();

		if (actorName.StartsWith("OptionsPlaceable"))
		{
			optionsPlacable = *v_itr;
			v_itr->SetActorLocation(hiddenLocation);
			UE_LOG(LogWindows, Warning, TEXT("found option menu"));
		}
		else if (actorName.StartsWith("LevelSelectPlaceable"))
		{
			levelSelectPlacable = *v_itr;
			v_itr->SetActorLocation(hiddenLocation);
			UE_LOG(LogWindows, Warning, TEXT("found level select menu"));
		}
		else if (actorName.StartsWith("CreditsPlaceable"))
		{
			creditsPlacable = *v_itr;
			v_itr->SetActorLocation(hiddenLocation);
			UE_LOG(LogWindows, Warning, TEXT("found credits menu"));
		}
		else if (actorName.StartsWith("MainMenu"))
		{
			mainMenuPlacable = *v_itr;
			currentMenu = mainMenuPlacable;
			UE_LOG(LogWindows, Warning, TEXT("found main menu menu"));
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
	SetWidgetVisibility(currentMenu, EVisibility::Hidden);
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
	TransitionToMenu(mainMenuPlacable, mainMenuShowLocation);
}

void AMainMenuContainer::ToOptions()
{
	//currentMenuState = MenuState::OPTIONS;
	TransitionToMenu(optionsPlacable, optionMenuShowLocation);
}

void AMainMenuContainer::ToLevelSelect()
{
	//currentMenuState = MenuState::LEVEL_SELECT;
	TransitionToMenu(levelSelectPlacable, levelSelectMenuShowLocation);
}

void AMainMenuContainer::ToCredits()
{
	//currentMenuState = MenuState::CREDITS;
	/*TransitionToMenu(creditsPlacable, creditMenuShowLocation);*/
	for (TActorIterator<AEndTile> v_itr(GetWorld()); v_itr; ++v_itr)
	{
		v_itr->loadLevelName = TEXT("DCredits");
		break;
	}

	StartNewGame();
}

void AMainMenuContainer::TransitionToMenu(AActor* menuPlaceable, const FVector& pos)
{
	if (menuPlaceable != currentMenu)
	{
		menuPlaceable->SetActorLocation(pos);
		currentMenu->SetActorLocation(hiddenLocation);
		currentMenu = menuPlaceable;
	}
}

UWidgetComponent* menuWidget;
void AMainMenuContainer::SetWidgetVisibility(AActor* widget, EVisibility visibility)
{
	auto component = widget->GetRootComponent();
	menuWidget = Cast<UWidgetComponent>(component);
	if (menuWidget != nullptr)
	{
		auto w = menuWidget->GetSlateWidget()->GetChildren()->GetChildAt(0);

		if (visibility.IsVisible())
		{
			widget->SetActorScale3D(FVector(1.0f));
		}
		
		if (!visibility.IsVisible())
		{
			widget->SetActorScale3D(FVector(0.0f));
		}
	}
}

FIntPoint AMainMenuContainer::getResolutionByString(const FString& screen)
{
	FIntPoint point(1920, 1080);

	if (screen.Equals("1920X1080"))
	{
		point = FIntPoint(1920, 1080);
	}
	else if (screen.Equals("1600X900"))
	{
		point = FIntPoint(1600, 900);
	}
	else if (screen.Equals("1366X768"))
	{
		point = FIntPoint(1366, 768);

	}
	else if (screen.Equals("1280X720"))
	{
		point = FIntPoint(1280, 720);

	}
	else if (screen.Equals("1024X576"))
	{
		point = FIntPoint(1024, 576);
	}

	return point;
}

FString AMainMenuContainer::getCurrentResolutionString()
{
	FString resolution = "1280X720";

	auto resoPoint = GEngine->GameUserSettings->GetScreenResolution();

	if (resoPoint == FIntPoint(1920, 1080))
	{
		resolution = "1920X1080";
	}
	else if (resoPoint == FIntPoint(1600, 900))
	{
		resolution = "1600X900";
	}
	else if (resoPoint == FIntPoint(1366, 768))
	{
		resolution = "1366X768";
	}
	else if (resoPoint == FIntPoint(1280, 720))
	{
		resolution = "1280X720";
	}
	else if (resoPoint == FIntPoint(1024, 576))
	{
		resolution = "1024X576";
	}

	return resolution;
}


void AMainMenuContainer::ChangeSettings(const FString& graphicsSetting,
									    const FString& resolution,
										bool fullScreen)
{
	auto screenMode = EWindowMode::Windowed ;
	auto settings = GEngine->GameUserSettings;
	settings->SetFullscreenMode(screenMode);
	auto resoPoint = getResolutionByString(resolution);
	settings->SetScreenResolution(resoPoint);
	SetGraphicsScalability(graphicsSetting, settings);

	settings->ApplySettings(false);
}

void AMainMenuContainer::SetGraphicsScalability(const FString& quality, UGameUserSettings* settings)
{
	if (quality.ToLower().Equals("low"))
	{
		SetGraphicsLow(settings);
	}
	else if (quality.ToLower().Equals("mid"))
	{
		SetGraphicsMid(settings);
	}
	else if (quality.ToLower().Equals("high"))
	{
		SetGraphicsHigh(settings);
	}
	else if (quality.ToLower().Equals("ultra"))
	{
		SetGraphicsUltra(settings);
	}
}

void AMainMenuContainer::SetGraphcisScalabilityNumber(UGameUserSettings* settings, int32 quality)
{
	settings->ScalabilityQuality.SetFromSingleQualityLevel(quality);
	if (quality == 0)
	{
		settings->ScalabilityQuality.ShadowQuality = 1;
	}
}


void AMainMenuContainer::SetGraphicsLow(UGameUserSettings* settings)
{
	SetGraphcisScalabilityNumber(settings, 0); 
}

void AMainMenuContainer::SetGraphicsMid(UGameUserSettings* settings)
{
	SetGraphcisScalabilityNumber(settings, 1);

}

void AMainMenuContainer::SetGraphicsHigh(UGameUserSettings* settings)
{
	SetGraphcisScalabilityNumber(settings, 2);
	settings->ScalabilityQuality.ResolutionQuality = 100;
}


void AMainMenuContainer::SetGraphicsUltra(UGameUserSettings* settings)
{
	SetGraphcisScalabilityNumber(settings, 3);

}
