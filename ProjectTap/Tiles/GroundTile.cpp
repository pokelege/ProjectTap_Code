// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GroundTile.h"
#include "UnrealType.h"
#include "IGroundable.h"
#include "GroundTileManager.h"
#include "Tile.h"
#if WITH_EDITOR
#include "UnrealEd.h"
#endif
// Sets default values
AGroundTile::AGroundTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Meshes" ) );
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh( TEXT( "/Game/Models/GroundTiles/1x1" ) );
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh2( TEXT( "/Game/Models/GroundBall" ) );
	Mesh->SetStaticMesh( mesh.Object );
	Mesh->SetMobility( EComponentMobility::Static );
	SetRootComponent( Mesh );
	Mesh->SetVisibility( GroundVisible );
	SelectAssistMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "SelectAssistMesh" ) );
	SelectAssistMesh->SetStaticMesh( mesh2.Object );
	SelectAssistMesh->SetMobility( EComponentMobility::Static );
	SelectAssistMesh->AttachTo(GetRootComponent());
	SelectAssistMesh->SetHiddenInGame(true);
	SelectAssistMesh->SetVisibility( !GroundVisible );
	SelectAssistMesh->SetCastShadow(false);
	SelectAssistMesh->bAffectDynamicIndirectLighting = false;
	SelectAssistMesh->bAffectDistanceFieldLighting = false;
	SelectAssistMesh->bCastDynamicShadow = false;
	SelectAssistMesh->bCastDynamicShadow = false;
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
			SetGroundVisible(stuff->defaultGroundVisibility);
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

void AGroundTile::SetGroundVisible(const bool isVisible)
{
	GroundVisible = isVisible;
	SelectAssistMesh->SetVisibility(!GroundVisible);
	Mesh->SetVisibility(GroundVisible);
}

void AGroundTile::GenerateActor()
{
	if ( ActorToAttach != nullptr && ( ActorToCreate == nullptr || !ActorToCreate->StaticClass()->GetFullName().Equals( ActorToAttach->StaticClass()->GetFullName() ) ) )
	{
		ActorToAttach->Destroy();
		ActorToAttach = nullptr;
		ActorToCreate = nullptr;
	}
	ActorToCreate = ActorToCreate == nullptr ? nullptr : ActorToCreate->IsChildOf<AActor>() ? ActorToCreate : nullptr;
	if ( ActorToCreate != nullptr )
	{
		ActorToAttach = GetWorld()->SpawnActor<AActor>( ActorToCreate , FVector::ZeroVector , FRotator::ZeroRotator );
		AttachActor();
	}
}

#if WITH_EDITOR
void AGroundTile::EditorKeyPressed( FKey Key ,
									EInputEvent Event )
{
	Super::EditorKeyPressed( Key , Event );
	if ( !IsSelected() ) return;
	if ( Cast<AGroundTileManager>( GetAttachParentActor() ) != nullptr && Key == EKeys::Enter && Event == EInputEvent::IE_Released )
	{
		GEditor->SelectNone( true , true );
		GEditor->SelectActor( GetAttachParentActor() , true , true );
	}
	if ( Event == EInputEvent::IE_Released && ( Key == EKeys::LeftShift || Key == EKeys::RightShift ) )
	{
		GenerateActor();
	}
}
void AGroundTile::PostEditChangeProperty( FPropertyChangedEvent & PropertyChangedEvent )
{
	Super::PostEditChangeProperty( PropertyChangedEvent );
	if ( PropertyChangedEvent.Property == nullptr ) return;
	if ( PropertyChangedEvent.Property->GetNameCPP().Equals( FString( "GroundVisible" ) ) )
	{
		Mesh->SetVisibility( GroundVisible );
		SelectAssistMesh->SetVisibility( !GroundVisible );
	}
	if ( PropertyChangedEvent.Property->GetNameCPP().Equals( FString( "ActorToAttach" ) ) )
	{
		if ( ActorToAttach != nullptr )
		{
			AttachActor();
		}
		else
		{
			SetGroundVisible(true);
		}
		ActorToCreate = ActorToAttach == nullptr ? nullptr : ActorToAttach->StaticClass();
	}
	if ( PropertyChangedEvent.Property->GetNameCPP().Equals( FString( "GenerateActorButton" ) ) || PropertyChangedEvent.Property->GetNameCPP().Equals( FString( "ActorToCreate" ) ) )
	{
		GenerateActor();
	}
}
#endif
