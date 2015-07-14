// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "BallPlayerStart.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API ABallPlayerStart : public APlayerStart
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
	FVector initialVelocity = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class AActor* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool followPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool lagCamera = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float lagSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float lagMaxDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool lockX = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool lockY = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool lockZ = true;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
	USoundBase* music = nullptr;
};
