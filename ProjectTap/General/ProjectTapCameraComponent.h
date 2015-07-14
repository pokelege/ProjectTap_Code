// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "ProjectTapCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTAP_API UProjectTapCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
private:
	enum FadeType {None,In,Out};
	FadeType currentFade = FadeType::None;
	float time = 0;
	void PlayIn( float DeltaSeconds );
	void PlayOut( float DeltaSeconds );
public:
	DECLARE_EVENT( AProjectTapCameraActor , FCameraFadedIn)
	FCameraFadedIn OnFadeIn;
	DECLARE_EVENT( AProjectTapCameraActor , FCameraFadedOut )
	FCameraFadedOut OnFadeOut;
	UFUNCTION()
	bool FadeIn();
	UFUNCTION()
	bool FadeOut();
	void ReceiveTick( float DeltaSeconds );
};
