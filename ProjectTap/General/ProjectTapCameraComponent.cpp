// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapCameraComponent.h"


UProjectTapCameraComponent::UProjectTapCameraComponent()
{
	bConstrainAspectRatio = true;
	PrimaryComponentTick.bCanEverTick = true;
	PostProcessSettings.bOverride_BloomIntensity = true;
	PostProcessSettings.BloomIntensity = 0.25f;
	PostProcessSettings.bOverride_BloomThreshold = true;
	PostProcessSettings.BloomThreshold = 3.5f;
}

bool UProjectTapCameraComponent::FadeIn()
{
	if ( currentFade != FadeType::None ) return false;
	currentFade = FadeType::In;
	time = 0;
	return true;
}
bool UProjectTapCameraComponent::FadeOut()
{
	if ( currentFade != FadeType::None ) return false;
	currentFade = FadeType::Out;
	time = 0;
	return true;
}

void UProjectTapCameraComponent::PlayIn( float DeltaSeconds )
{
	time += DeltaSeconds;
	float fadeValue = FMath::Clamp<float>( time / 1 , 0 , 1 );
	PostProcessSettings.bOverride_ColorGain = true;
	PostProcessSettings.ColorGain = FVector( fadeValue , fadeValue , fadeValue );
	OnFadeUpdate.Broadcast( fadeValue );
	if ( time >= 1 )
	{
		time = 0;
		OnFadeIn.Broadcast();
		currentFade = FadeType::None;
	}
}
void UProjectTapCameraComponent::PlayOut( float DeltaSeconds )
{
	time += DeltaSeconds;
	float fadeValue = 1 - FMath::Clamp<float>( time / 1 , 0 , 1 );
	PostProcessSettings.bOverride_ColorGain = true;
	PostProcessSettings.ColorGain = FVector( fadeValue , fadeValue , fadeValue );
	OnFadeUpdate.Broadcast(fadeValue);
	if ( time >= 1 )
	{
		time = 0;
		OnFadeOut.Broadcast();
		currentFade = FadeType::None;
	}

}

void UProjectTapCameraComponent::TickComponent( float DeltaSeconds ,ELevelTick TickType ,
												FActorComponentTickFunction * ThisTickFunction )
{
	switch ( currentFade )
	{
		case FadeType::In :
			PlayIn( DeltaSeconds );
			break;
		case FadeType::Out:
			PlayOut(DeltaSeconds);
			break;
	}
}
