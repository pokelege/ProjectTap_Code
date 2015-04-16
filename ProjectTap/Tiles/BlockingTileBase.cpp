// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BlockingTileBase.h"


// Sets default values
ABlockingTileBase::ABlockingTileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision->SetBoxExtent(FVector(1.0f, 1.0f, 1.0f));
}

// Called when the game starts or when spawned
void ABlockingTileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlockingTileBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	auto pos = GetActorLocation();
	bool canRise = pos.Z - original.Z < move_distance_tolerance;
	bool canDesend = pos.Z - original.Z > FLT_EPSILON;

	if (activated && canRise)
	{
		pos.Z += move_speed * DeltaTime;
	}
	else if (!activated && canDesend)
	{
		pos.Z -= move_speed * DeltaTime;
	}
	else if (!activated)
	{
		pos.Z = original.Z;
	}

	SetActorLocation(pos);

	TileMesh->SetRenderCustomDepth(true);
}

