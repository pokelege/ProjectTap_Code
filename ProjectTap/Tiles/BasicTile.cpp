// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BasicTile.h"

ABasicTile::ABasicTile()
{
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile mesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/Models/Tile"));
	TileMesh->SetStaticMesh(mesh.Object);
	TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(TileMesh);
}