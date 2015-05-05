// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MovingTile.h"


// Sets default values
AMovingTile::AMovingTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile mesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/Models/Tile"));
	TileMesh->SetStaticMesh(mesh.Object);
	TileMesh->SetWorldScale3D(FVector(40.0f, 40.0f, 10.0f));
	TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AMovingTile::BeginPlay()
{
	Super::BeginPlay();
	
	if (path.Num() > 0)
	{
		SetActorLocation(path[currNode]);
	}
}

void AMovingTile::reset()
{
	pauseTimeCounter = 0.0f;
	beta = 0.0f;
	currDir = NextDirection();
}


// Called every frame
void AMovingTile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	UpdateMovement(DeltaTime);
}


void AMovingTile::UpdateMovement(float dt)
{
	auto next = NextIndex();
	auto nextPos = path[next];
	bool reachedNext = FVector::Dist(GetActorLocation(), nextPos) < 1.0f;

	if (!reachedNext)
	{
		auto newCurr = GetActorLocation() + currDir * speed * dt;
		SetActorLocation(newCurr);

		if (carryOn != nullptr)
		{
			carryOn->SetActorLocation(newCurr);
		}
	}
	else if (pauseTimeCounter < pauseBetweenNodes)
	{
		SetActorLocation(nextPos);
		pauseTimeCounter += dt;
	}
	else
	{
		IncrementIndex();
		reset();

	}

}

unsigned AMovingTile::NextIndex()
{
	bool isBegining = currNode == 0 && pathReversed;
	bool isEnd = currNode == path.Num() - 1 && !pathReversed;

	if (isBegining || isEnd)
	{
		pathReversed = !pathReversed;
	}

	if (pathReversed)
	{
		return currNode - 1;
	}
	else
	{
		return  currNode + 1;
	}
}

unsigned AMovingTile::IncrementIndex()
{
	bool isBegining = currNode == 0;
	bool isEnd = currNode == path.Num() - 1;

	if (isBegining || isEnd)
	{
		pathReversed = !pathReversed;
	}

	if (pathReversed)
	{
		return --currNode;
	}
	else
	{
		return  ++currNode;
	}
}


FVector AMovingTile::NextDirection()
{
	auto currPos = path[currNode];
	auto nextPos = path[NextIndex()];

	return (nextPos - currPos).GetSafeNormal();
}
