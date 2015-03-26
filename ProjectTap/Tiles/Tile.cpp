// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Tile.h"

// Sets default values
ATile::ATile(const FObjectInitializer& initializer) :Super(initializer)
{
	TileMesh = initializer.CreateDefaultSubobject<UStaticMeshComponent>( this , TEXT( "Tile mesh" ) );
	TileMesh->SetWorldScale3D( FVector( 1 , 1 , 0.1f ) );
	this->SetRootComponent( TileMesh );
}

bool ATile::Touched()
{
	if ( isTouchable )
	{
		UE_LOG( LogTemp , Warning , TEXT( "Touched" ) );
		return true;
	}
	else
	{
		UE_LOG( LogTemp , Warning , TEXT( "Can't be touched" ) );
		return false;
	}
}

bool ATile::BallTouched()
{
	if ( isBallTouchable )
	{
		UE_LOG( LogTemp , Warning , TEXT( "Ball Touched" ) );
		return true;
	}
	else
	{
		UE_LOG( LogTemp , Warning , TEXT( "Can't be ball touched" ) );
		return false;
	}
}
