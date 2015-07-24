// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GroundTile.h"
#include "UnrealType.h"
// Sets default values
AGroundTile::AGroundTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Meshes" ) );
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh( *FName( "/Game/Models/GroundTiles/1x1" ).ToString() );
	Mesh->SetStaticMesh( mesh.Object );
	SetRootComponent( Mesh );
	Mesh->SetVisibility( GroundVisible );
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
			ActorToAttach->SetActorRelativeLocation(FVector(0,0,80));
			ActorToAttach->SetActorRelativeRotation(FRotator(0));
			ActorToAttach->SetActorRelativeScale3D(FVector(1));
		}
	}
}
#endif