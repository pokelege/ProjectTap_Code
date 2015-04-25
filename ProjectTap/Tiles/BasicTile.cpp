// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BasicTile.h"




ABasicTile::ABasicTile() :ATile()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*FName("/Game/Models/Tile").ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(40.0f, 40.0f, 80.0f));
	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(1,1,1), false);
	}
}