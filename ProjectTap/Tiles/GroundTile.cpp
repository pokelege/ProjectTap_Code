// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GroundTile.h"
#include "UnrealType.h"
#include "IGroundable.h"
#include "GroundTileManager.h"
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

void AGroundTile::AttachActor()
{
	auto otherTile = Cast<AGroundTile>( ActorToAttach->GetAttachParentActor() );
	if ( otherTile != nullptr && otherTile != this )
		otherTile->ActorToAttach = nullptr;
	ActorToAttach->AttachRootComponentToActor( this );
	UpdateAttachedLocation();
	ActorToAttach->SetActorRelativeRotation( FRotator( 0 ) );
	ActorToAttach->SetActorRelativeScale3D( FVector( 1 ) );
}

void AGroundTile::Destroyed()
{
	if ( DestroyActorWithGround && ActorToAttach != nullptr )
	{
		ActorToAttach->Destroy();
	}
	Super::Destroyed();
}

#if WITH_EDITOR
void AGroundTile::EditorKeyPressed( FKey Key ,
									EInputEvent Event )
{
	Super::EditorKeyPressed(Key,Event);
	if ( Cast<AGroundTileManager>( GetAttachParentActor() ) != nullptr && IsSelected() && Key == EKeys::Enter && Event == EInputEvent::IE_Released )
	{
		//todo select parent
	}
}
void AGroundTile::PostEditChangeProperty( FPropertyChangedEvent & PropertyChangedEvent )
{
	Super::PostEditChangeProperty( PropertyChangedEvent );
	if ( PropertyChangedEvent.Property == nullptr ) return;
	if ( PropertyChangedEvent.Property->GetNameCPP().Equals( FString( "GroundVisible" ) ) )
	{
		Mesh->SetVisibility( GroundVisible );
	}
	if ( PropertyChangedEvent.Property->GetNameCPP().Equals( FString( "ActorToAttach" ) ) )
	{
		if ( ActorToAttach != nullptr )
		{
			AttachActor();
		}
		ActorToCreate = ActorToAttach == nullptr ? nullptr : ActorToAttach->StaticClass();
	}
	if ( PropertyChangedEvent.Property->GetNameCPP().Equals( FString( "GenerateActorButton" ) ) )
	{
		if ( ActorToAttach != nullptr && ( ActorToCreate == nullptr || !ActorToCreate->StaticClass()->GetFullName().Equals( ActorToAttach->StaticClass()->GetFullName() ) ) )
		{
			ActorToAttach->Destroy();
			ActorToAttach = nullptr;
		}
		ActorToCreate = ActorToCreate == nullptr ? nullptr : ActorToCreate->IsChildOf<AActor>() ? ActorToCreate : nullptr;
		if ( ActorToCreate != nullptr )
		{
			ActorToAttach = GetWorld()->SpawnActor<AActor>( ActorToCreate , FVector::ZeroVector , FRotator::ZeroRotator );
			AttachActor();
		}
	}
}
#endif
