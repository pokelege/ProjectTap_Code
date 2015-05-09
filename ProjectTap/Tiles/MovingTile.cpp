// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MovingTile.h"
#include "../OffensiveTiles/Laser.h"


// Sets default values
AMovingTile::AMovingTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile mesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/Models/MoveTile"));
	TileMesh->SetStaticMesh(mesh.Object);
	TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TileMesh->SetWorldScale3D(FVector(1.0f));

	
	BoxCollision->SetBoxExtent(FVector(1.0f));
	BoxCollision->SetWorldScale3D(FVector(40.0f, 40.0f, 10.0f));

	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Game/Curves/MovementLinear"));
	if (curve.Object != nullptr) moveCurve = curve.Object;
	
}

// Called when the game starts or when spawned
void AMovingTile::BeginPlay()
{
	Super::BeginPlay();
	
	if (path.Num() > 1 && enabled)
	{
		currDir = (path[NextIndex()] - path[currNode]).GetSafeNormal();
		SetActorLocation(path[currNode]);
	}

	if (carryOn != nullptr)
	{
		Children.Add(carryOn);
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
	UpdateCarryOn(DeltaTime);

}

void AMovingTile::UpdateMovement(float dt)
{
	if (enabled)
	{
		auto next = NextIndex();
		auto nextPos = path[next];
		auto remainingDistSq = FVector::DistSquared(GetActorLocation(), nextPos);
		auto checkDirection = nextPos - GetActorLocation();
		bool reachedNext = FVector::DotProduct(checkDirection, currDir) < 0.0f;

		auto distanceBetweenCurrentNodes = FVector::DistSquared(nextPos, path[currNode]);
		float min, max;

		moveCurve->GetTimeRange(min, max);

		if (!reachedNext)
		{
			auto speedFactor = moveCurve->GetFloatValue(1.0f - remainingDistSq / distanceBetweenCurrentNodes);
			auto newCurr = GetActorLocation() + currDir * speed * speedFactor * dt;
			SetActorLocation(newCurr);
		}                     
		else if (pauseTimeCounter < pauseBetweenNodes)
		{
			if (pauseTimeCounter == 0.0f)
			{
				SetActorLocation(nextPos);
			}
			pauseTimeCounter += dt;
		}
		else
		{
			IncrementIndex();
			reset();
		}
	}
}

int32 AMovingTile::NextIndex()
{
	int dir = pathReversed ? -1 : 1;

	return currNode + dir;
}

int32 AMovingTile::IncrementIndex()
{
	bool exceedEnd = NextIndex() >= path.Num() - 1;
	bool exceedBegining = NextIndex() == 0;

	currNode = NextIndex();

	if (exceedEnd || exceedBegining)
	{
		pathReversed = !pathReversed;
	}

	return currNode;
}


FVector AMovingTile::NextDirection()
{
	auto currPos = path[currNode];
	auto nextPos = path[NextIndex()];

	return (nextPos - currPos).GetSafeNormal();
}

void AMovingTile::UpdateCarryOn(float dt)
{
	if (auto tile = Cast<ATile>(carryOn))
	{
		tile->SetActorLocation(GetActorLocation());
	}
	else if (auto laser = Cast<ALaser>(carryOn))
	{
		laser->SetLaserLocation(GetActorLocation());
	}
}

