// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "CustomGameMode.h"
#include "Tiles/IGroundable.h"
#include "BallPlayerStart.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API ABallPlayerStart : public APlayerStart, public IGroundable
{
	GENERATED_BODY()
	
	static const GroundableInfo groundableInfo;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Ball )
		FVector initialVelocity = FVector( 0.0f , 0.0f , 0.0f );
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
		USoundBase* music = nullptr;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Camera )
	class AProjectTapCamera* camera;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Camera )
		float lagSpeed = 1.0f;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Camera )
		float lagMaxDistance = 100.0f;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = GameMode )
		CustomGameMode GameMode = CustomGameMode::GAME_MODE_LEVEL;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Camera )
		bool followPlayer = false;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Camera )
		bool lagCamera = false;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Camera )
		bool lockX = false;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Camera )
		bool lockY = false;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Camera )
		bool lockZ = true;

	virtual const struct GroundableInfo* GetGroundableInfo() const override;
};
