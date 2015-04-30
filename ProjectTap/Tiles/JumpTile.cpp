// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "JumpTile.h"




void AJumpTile::BeginPlay()
{
	Super::BeginPlay();
}

void AJumpTile::activate()
{
	if(rotationSequence == nullptr || target == nullptr || ball == nullptr || activated) return;
	Super::activate();
	//todo check if mass is the same
	calculatePhysics();
	ball->AddVelocity(force);
}
void AJumpTile::calculatePhysics()
{
	
}