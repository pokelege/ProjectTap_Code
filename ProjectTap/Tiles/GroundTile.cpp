// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GroundTile.h"


// Sets default values
AGroundTile::AGroundTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Collision = CreateDefaultSubobject<UBoxComponent>( TEXT( "Collision" ) );
	SetRootComponent( Collision );

	Collision->SetCollisionResponseToAllChannels( ECollisionResponse::ECR_Ignore );
	Collision->SetCollisionResponseToChannel( ECollisionChannel::ECC_Pawn , ECollisionResponse::ECR_Block );
	Collision->SetCollisionResponseToChannel( ECollisionChannel::ECC_WorldStatic , ECollisionResponse::ECR_Block );
	Collision->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	Collision->SetCollisionObjectType( ECollisionChannel::ECC_WorldStatic );
	Collision->SetMobility( EComponentMobility::Static );

	Generate();
}

void AGroundTile::Generate()
{
	FVector boxExtent( ( float ) NumTilesX * MeshScaleX , ( float ) NumTilesY * MeshScaleY , MeshScaleZ );
	Collision->SetBoxExtent( boxExtent );

	for ( int x = 0; x < NumTilesX; ++x )
	{
		for ( int y = 0; y < NumTilesY; ++y )
		{
			bool ignoreMesh = false;
			for ( int currentIgnoreMesh = 0; currentIgnoreMesh < IgnoredMeshes.Num(); ++currentIgnoreMesh )
			{
				FVector currentVector = IgnoredMeshes[currentIgnoreMesh];
				ignoreMesh = ( ( ( int ) currentVector.X ) == x && ( ( int ) currentVector.Y ) == y );
				if ( ignoreMesh )
				{
					break;
				}
			}
			if ( ignoreMesh ) continue;
		}
	}
}