// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
/// <summary>
/// The base tile
/// </summary>
class PROJECTTAP_API ATile : public AActor
{
	GENERATED_BODY()

public:	
	/// <summary>
	/// Bool for if can be clicked.
	/// </summary>
	bool isTouchable;
	
	/// <summary>
	/// Bool for if ball can activate tile.
	/// </summary>
	bool isBallTouchable;

	/// <summary>
	/// Called if touched by player
	/// </summary>
	virtual bool Touched();

	/// <summary>
	/// Called if touched by ball
	/// </summary>
	virtual bool BallTouched();

	// Sets default values for this actor's properties
	ATile( const FObjectInitializer& initializer );

protected:
	UStaticMeshComponent* TileMesh;
};