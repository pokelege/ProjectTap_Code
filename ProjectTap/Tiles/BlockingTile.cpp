// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BlockingTile.h"
#include "../Utils/LoadAssetsHelper.h"

// Sets default values
ABlockingTile::ABlockingTile(const FObjectInitializer& initializer ) : Super(initializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FName path("/Game/Models/BlockingTile");
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	TileMesh->SetWorldScale3D(FVector(.5f, .5f, 5.0f));
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));

	BoxCollision->AttachTo(RootComponent);
	BoxCollision->SetBoxExtent(FVector(20.0f, 20.0f, 20.0f));
	BoxCollision->SetWorldScale3D(FVector(6.0f, 6.0f, 6.4f));

	//SetActorScale3D(FVector(.25f, 0.25f, .1f));

}

// Called when the game starts or when spawned
void ABlockingTile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABlockingTile::Tick( float DeltaTime )
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
	else if(!activated)
	{
		pos.Z = original.Z;
	}

	SetActorLocation(pos);
}