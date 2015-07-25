// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GroundTile.h"
#include "UnrealType.h"
#include "IGroundable.h"
// Sets default values
AGroundTile::AGroundTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Meshes" ) );
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh( *FName( "/Game/Models/GroundTiles/1x1" ).ToString() );
	Mesh->SetStaticMesh( mesh.Object );
	Mesh->SetMobility( EComponentMobility::Static );
	SetRootComponent( Mesh );
	Mesh->SetVisibility( GroundVisible );
}

void AGroundTile::UpdateAttachedLocation()
{
	if ( ActorToAttach != nullptr )
	{
		ActorToAttach->SetActorRelativeLocation( FVector( 0 , 0 , 80 ) );
		IGroundable* groundableActor;
		if ( ( groundableActor = Cast<IGroundable>( ActorToAttach ) ) != nullptr )
		{
			auto stuff = groundableActor->GetGroundableInfo();
			ActorToAttach->AddActorLocalOffset( stuff->offset );
			GroundVisible = stuff->defaultGroundVisibility;
			Mesh->SetVisibility( GroundVisible );
		}
	}
}

#if WITH_EDITOR
void AGroundTile::PostEditChangeProperty( FPropertyChangedEvent & PropertyChangedEvent )
{
	Super::PostEditChangeProperty( PropertyChangedEvent );
	if ( PropertyChangedEvent.Property->GetNameCPP().Equals( FString( "GroundVisible" ) ) )
	{
		Mesh->SetVisibility( GroundVisible );
	}
	if ( PropertyChangedEvent.Property->GetNameCPP().Equals( FString( "ActorToAttach" ) ) )
	{
		if ( ActorToAttach != nullptr )
		{
			ActorToAttach->AttachRootComponentToActor(this);
			UpdateAttachedLocation();
			ActorToAttach->SetActorRelativeRotation(FRotator(0));
			ActorToAttach->SetActorRelativeScale3D(FVector(1));
		}
	}
}
#endif
