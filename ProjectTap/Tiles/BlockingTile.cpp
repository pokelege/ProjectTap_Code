// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BlockingTile.h"

// Sets default values
ABlockingTile::ABlockingTile(const FObjectInitializer& initializer ) : Super(initializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	
	RootComponent = TileMesh;

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));

	boxCollision->AttachTo(RootComponent);

	boxCollision->SetBoxExtent(FVector(20.0f, 20.0f, 20.0f));

	SetActorScale3D(FVector(5.0f, 5.0f, 10.0f));
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
	bool canRise = pos.Z - original < move_distance_tolerance;
	bool canDesend = pos.Z - original > FLT_EPSILON;

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
		pos.Z = original;
	}

	SetActorLocation(pos);
}