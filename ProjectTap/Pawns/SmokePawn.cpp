// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "SmokePawn.h"
#include "Controllers/SmokeAIController.h"

// Sets default values
ASmokePawn::ASmokePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ASmokeAIController::StaticClass();
}

// Called when the game starts or when spawned
void ASmokePawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASmokePawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ASmokePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

