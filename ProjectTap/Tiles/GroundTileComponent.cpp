// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GroundTileComponent.h"

UGroundTileComponent::UGroundTileComponent()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh( *FName( "/Game/Models/GroundTiles/1x1" ).ToString() );
	SetStaticMesh(mesh.Object);
}

#if WITH_EDITOR
void UGroundTileComponent::PostEditChangeProperty( FPropertyChangedEvent & PropertyChangedEvent )
{
	Super::PostEditChangeProperty( PropertyChangedEvent );
}
#endif