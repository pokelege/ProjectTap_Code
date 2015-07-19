// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MovingTile.h"
#include "ICarriable.h"
#include "OffensiveTiles/Laser.h"
#include "Runtime/CoreUObject/Public/UObject/UnrealType.h"

// Sets default values
AMovingTile::AMovingTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile mesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/Models/MoveTile"));
	
	TileMesh->SetStaticMesh(mesh.Object);
	BoxCollision->SetWorldScale3D(FVector(1));
	BoxCollision->SetBoxExtent(FVector(40.0f, 40.0f, 10.0f));


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
	if (enabled && path.Num() > 0)
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

			checkDirection = nextPos - GetActorLocation();

			//chekc if went over destination
			if (FVector::DotProduct(currDir, checkDirection.GetSafeNormal()) < 0.0f)
			{
				SetActorLocation(nextPos);
				IncrementIndex();
				reset();
			}
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
	if (reverseRouteWhenDone)
	{
		int dir = pathReversed ? -1 : 1;
		return currNode + dir;
	}
	else
	{
		return (currNode + 1) % path.Num();
	}
}

int32 AMovingTile::IncrementIndex()
{
	bool exceedEnd = NextIndex() >= path.Num() - 1;
	bool exceedBegining = NextIndex() == 0;

	currNode = NextIndex();

	if ((exceedEnd || exceedBegining) && reverseRouteWhenDone)
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
	auto pos = GetActorLocation();

	if (carryOn != nullptr)
	{		
		if (auto actor = Cast<ICarriable>(carryOn))
		{
			auto info = actor->getOffsetInfo();
			auto newPos = GetActorLocation() + info.offsetForCarryOn;
			carryOn->SetActorLocation(newPos);

			if (auto laser = Cast<ALaser>(carryOn))
			{
				laser->SetLaserLocationWithDefaultHitLocation(newPos);
			}

		}
	}
}


#if WITH_EDITOR
void AMovingTile::EditorKeyPressed(FKey Key, EInputEvent Event)
{
	Super::EditorKeyPressed(Key, Event);
	auto keyname = Key.GetFName().ToString();

	if (Event == EInputEvent::IE_Released)
	{
		if (keyname.Equals("Enter"))
		{
			AddCurrentLocation();
		}
		else if (keyname.Equals("LeftShift") || keyname.Equals("RightShift"))
		{
			UpdateCurrentLocation();
		}
	}
}

void AMovingTile::AddCurrentLocation()
{
	path.Add(GetActorLocation());
	currentEditorPathIndex = path.Num() - 1;
}

void AMovingTile::UpdateCurrentLocation()
{
	path[currentEditorPathIndex] = GetActorLocation();
}

void AMovingTile::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		auto p = PropertyChangedEvent.Property;
		auto pName = p->GetName();

		//when currentEditorPathIndex property changes in editor
		//reset current moving tile's location to desinated node's location
		if (pName.Equals(TEXT("currentEditorPathIndex")))
		{
			auto intProp = Cast<UIntProperty>(p); 

			if (currentEditorPathIndex >= 0 && currentEditorPathIndex < path.Num())
			{
				auto newLocation = path[currentEditorPathIndex];
				SetActorLocation(newLocation);
			}
			else
			{
				currentEditorPathIndex = 0;
				auto newLocation = path[currentEditorPathIndex];
				SetActorLocation(newLocation);
			}
			Super::PostEditChange();

		}
	}
}


#endif 