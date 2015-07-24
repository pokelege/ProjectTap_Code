// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GroundTileManager.h"
#include "GroundTile.h"
// Sets default values
AGroundTileManager::AGroundTileManager()
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
}

void AGroundTileManager::Generate()
{
	FVector boxExtent( ( float ) NumTilesX * MeshScaleX , ( float ) NumTilesY * MeshScaleY , MeshScaleZ );
	Collision->SetBoxExtent( boxExtent );
	TArray<AGroundTile*> GroundTiles;
	for ( TActorIterator<AGroundTile> ActorItr( GetWorld() ); ActorItr; ++ActorItr )
	{
		if ( ActorItr->IsAttachedTo(this) ) GroundTiles.Add(*ActorItr);
	}
	auto meshesIndex = 0;
	
	for ( int x = 1; x <= NumTilesX; ++x )
	{
		for ( int y = 1; y <= NumTilesY; ++y )
		{
			AGroundTile* newGround = nullptr;
			if ( meshesIndex < GroundTiles.Num() )
			{
				newGround = Cast<AGroundTile>( GroundTiles[meshesIndex++] );
				newGround->AttachRootComponentToActor( this );
			}
			else
			{
				newGround = GetWorld()->SpawnActor<AGroundTile>();
				newGround->AttachRootComponentToActor(this);
				GroundTiles.Add(newGround);
				++meshesIndex;
			}
			FVector translation( 0 );
			//todo optimize if possible
			translation.X = ( ( x - 1 ) * ( MeshScaleX * 2.0f ) ) - ( ( ( NumTilesX - 1 ) * ( MeshScaleX * 2.0f ) ) / 2.0f );
			translation.Y = ( ( y - 1 ) * ( MeshScaleY * 2.0f ) ) - ( ( ( NumTilesY - 1 ) * ( MeshScaleY * 2.0f ) ) / 2.0f );
			newGround->SetActorRelativeLocation(translation);
			newGround->SetActorRelativeRotation(FRotator(0));
			newGround->SetActorRelativeScale3D(FVector(1));
		}
	}
	while ( meshesIndex < GroundTiles.Num() )
	{
		GroundTiles.Pop()->Destroy();
	}
}

#if WITH_EDITOR
void AGroundTileManager::PostEditChangeProperty( FPropertyChangedEvent & PropertyChangedEvent )
{
	//todo optimize
	Generate();
	Super::PostEditChangeProperty( PropertyChangedEvent );
}
#endif