// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
class ABallPawn;
#include "GameFramework/GameMode.h"

#include "ProjectTapGameMode.generated.h"



/**
 *
 */
UCLASS()
class PROJECTTAP_API AProjectTapGameMode : public AGameMode
{
    GENERATED_BODY()

	DECLARE_DELEGATE(OnPlayerDestroyDelegate)

    class ABallPawn* ball;

public:

	OnPlayerDestroyDelegate onPlayerDestroy;
    AProjectTapGameMode ( const FObjectInitializer& initializer );

    virtual void BeginPlay() override;

    virtual void Tick ( float DeltaTime ) override;

    void Respawn();
};
