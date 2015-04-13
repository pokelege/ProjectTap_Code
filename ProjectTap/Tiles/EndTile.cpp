// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "EndTile.h"

void AEndTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(BallCollision->IsOverlappingActor(ball))
	{
		//complete game
	}
}
