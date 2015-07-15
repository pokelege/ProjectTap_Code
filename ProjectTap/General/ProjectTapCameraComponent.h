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
	UProjectTapCameraComponent();
	DECLARE_MULTICAST_DELEGATE( FCameraFadedIn )
	FCameraFadedIn OnFadeIn;
	DECLARE_MULTICAST_DELEGATE( FCameraFadedOut )
	FCameraFadedOut OnFadeOut;
	DECLARE_MULTICAST_DELEGATE_OneParam( FCameraFadeUpdate , const float )
	FCameraFadeUpdate OnFadeUpdate;
	UFUNCTION()
	bool FadeIn();
	UFUNCTION()
	bool FadeOut();
	void TickComponent( float DeltaSeconds , enum ELevelTick TickType ,
					  FActorComponentTickFunction * ThisTickFunction );
};
