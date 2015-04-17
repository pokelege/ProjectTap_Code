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
};
