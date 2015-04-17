// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameMode.h"
#include "Pawns/BallPawn.h"
#include "Controllers/MouseController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
AProjectTapGameMode::AProjectTapGameMode( const FObjectInitializer& initializer ): Super( initializer )
{
	UE_LOG( LogTemp , Warning , TEXT( "mouse" ) );
	PlayerControllerClass = AMouseController::StaticClass();
	DefaultPawnClass = 0;
}

void AProjectTapGameMode::BeginPlay()
{
	if (UWorld* world = GetWorld())
	{
		FTransform playerStart = FindPlayerStart(0, FString("Player"))->GetTransform();
		FActorSpawnParameters params;
		//AActor* spawned = world->SpawnActor(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotation(playerStart.GetRotation());
		ball = world->SpawnActor<ABallPawn>(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotator(playerStart.GetRotation()), params);
	}
}

void AProjectTapGameMode::Respawn()
{
	if (ball != nullptr)
	{
		//Destroy(true);
		ball = nullptr;
	}

	if (UWorld* world = GetWorld())
	{
		FTransform playerStart = FindPlayerStart(0, FString("Player"))->GetTransform();
		FActorSpawnParameters params;
		//AActor* spawned = world->SpawnActor(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotation(playerStart.GetRotation());
		ball = world->SpawnActor<ABallPawn>(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotator(playerStart.GetRotation()), params);
	}
}