// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GroupedBlockingTile.h"


// Sets default values
AGroupedBlockingTile::AGroupedBlockingTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FName path("/Game/Models/GroupedBlockingTile");
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path.ToString());
	TileMesh->SetStaticMesh(mesh.Object);

	BoxCollision->SetBoxExtent(FVector(1.0f, 1.0f, 1.0f));
	BoxCollision->SetWorldScale3D(FVector(20.0f, 20.0f, 80.0f));
}

// Called when the game starts or when spawned
void AGroupedBlockingTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGroupedBlockingTile::Tick( float DeltaTime )
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

	//count time
	if (activated) {

		if (time_counter < activation_time)
		{
			time_counter += DeltaTime;
		} 
		else
		{
			deactivate();
		}
	}

}


void AGroupedBlockingTile::deactivate()
{
	time_counter = .0f;
	activated = false;
}

