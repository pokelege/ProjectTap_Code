// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapCameraComponent.h"




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
	if ( time >= 1 )
	{
		time = 0;
		OnFadeOut.Broadcast();
		currentFade = FadeType::None;
	}
}

void UProjectTapCameraComponent::ReceiveTick( float DeltaSeconds )
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
