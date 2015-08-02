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

	SetRootComponent(TileMesh);
	TileMesh->DetachFromParent();
	BoxCollision->AttachChildren.Remove(TileMesh);
	BoxCollision->AttachTo(TileMesh);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Game/Curves/MovementLinear"));
	if (curve.Object != nullptr) moveCurve = curve.Object;

}

// Called when the game starts or when spawned
void AMovingTile::BeginPlay()
{
	Super::BeginPlay();

	if (path.Num() > 1 && enabled)
	{
		if (currentPathIndex >= path.Num() - 1)
		{
			bool exceedEnd = NextIndex() >= path.Num() - 1;
			bool exceedBegining = NextIndex() == 0;

			if ((exceedEnd || exceedBegining) && reverseRouteWhenDone)
			{
				pathReversed = !pathReversed;
			}
		}
		auto next = NextIndex();
		currDir = (path[next] - path[currentPathIndex]).GetSafeNormal();
		SetActorLocation(path[currentPathIndex]);
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

	if (delayTimeCounter >= startDelay)
	{
		UpdateMovement(DeltaTime);
		UpdateCarryOn();
	}
	else
	{
		delayTimeCounter += DeltaTime;
	}
}

void AMovingTile::UpdateMovement(float dt)
{
	if (enabled && path.Num() > 1)
	{
		auto next = NextIndex();
		auto nextPos = path[next];
		auto remainingDistSq = FVector::DistSquared(GetActorLocation(), nextPos);
		auto checkDirection = nextPos - GetActorLocation();
		bool reachedNext = FVector::DotProduct(checkDirection, currDir) < 0.0f;

		auto distanceBetweenCurrentNodes = FVector::DistSquared(nextPos, path[currentPathIndex]);
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
			}
		}
	}
}

int32 AMovingTile::NextIndex()
{
	if (reverseRouteWhenDone)
	{
		int dir = pathReversed ? -1 : 1;
		return currentPathIndex + dir;
	}
	else
	{
		return (currentPathIndex + 1) % path.Num();
	}
}

int32 AMovingTile::IncrementIndex()
{
	if (pauseTimeCounter < pauseBetweenNodes)
	{
		pauseTimeCounter += GetWorld()->DeltaTimeSeconds;
	}
	else
	{
		bool exceedEnd = NextIndex() >= path.Num() - 1;
		bool exceedBegining = NextIndex() == 0;

		currentPathIndex = NextIndex();

		if ((exceedEnd || exceedBegining) && reverseRouteWhenDone)
		{
			pathReversed = !pathReversed;
		}


		reset();
	}

	return currentPathIndex;
}


FVector AMovingTile::NextDirection()
{
	auto currPos = path[currentPathIndex];
	auto nextPos = path[NextIndex()];

	return (nextPos - currPos).GetSafeNormal();
}

void AMovingTile::UpdateCarryOn()
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
		if (Key == EKeys::Enter)
		{
			AddCurrentLocation();
		}
		else if (Key == EKeys::C)
		{
			UpdateCurrentLocation();
		}
		else if (Key == EKeys::V)
		{
			DeleteCurrentLocation();
		}
	}
}

void AMovingTile::DeleteCurrentLocation()
{
	if (currentPathIndex >= 0 && currentPathIndex < path.Num())
	{
		path.RemoveAt(currentPathIndex);
		if (path.Num() == 0)
		{
			currentPathIndex = -1;
		}
		else
		{
			currentPathIndex = 0;			
			SetActorLocation(path[currentPathIndex]);
		}
	}
}

void AMovingTile::AddCurrentLocation()
{
	path.Add(GetActorLocation());
	currentPathIndex = path.Num() - 1;
}

void AMovingTile::UpdateCurrentLocation()
{
	if (path.Num() > 0)
	{
		path[currentPathIndex] = GetActorLocation();
	}
}

void AMovingTile::EditorApplyTranslation
		(
			const FVector & DeltaTranslation,
			bool bAltDown,
			bool bShiftDown,
			bool bCtrlDown
		)
{
	Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);

	if (auto tile = Cast<ICarriable>(carryOn))
	{
		auto info = tile->getOffsetInfo();
		carryOn->SetActorLocation(GetActorLocation() + info.offsetForCarryOn);
	}

}
void AMovingTile::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		auto p = PropertyChangedEvent.Property;
		auto pName = p->GetName();

		//when currentPathIndex property changes in editor
		//reset current moving tile's location to desinated node's location
		if (pName.Equals(TEXT("currentPathIndex")))
		{
			if (currentPathIndex >= 0 && currentPathIndex < path.Num())
			{
				auto newLocation = path[currentPathIndex];
				SetActorLocation(newLocation);

				UpdateCarryOn();
			}
			else
			{
				currentPathIndex = 0;
				auto newLocation = path[currentPathIndex];
				SetActorLocation(newLocation);
			}

			Super::PostEditChange();

		}
		else if (pName.Equals(TEXT("carryOn")))
		{
			UpdateCarryOn();
		}
		else if (pName.Equals(TEXT("ActorToCreate")))
		{
			GenerateActor();
		}
		else if (pName.Equals(TEXT("path")))
		{
			currentPathIndex = path.Num() > 0 ? path.Num() - 1 : 0;
		}
	}
}


void AMovingTile::GenerateActor()
{
	if (carryOn != nullptr && (ActorToCreate == nullptr || !ActorToCreate->StaticClass()->GetFullName().Equals(carryOn->StaticClass()->GetFullName())))
	{
		carryOn->Destroy();
		carryOn = nullptr;
		ActorToCreate = nullptr;
	}
	ActorToCreate = ActorToCreate == nullptr ? nullptr : ActorToCreate->IsChildOf<AActor>() ? ActorToCreate : nullptr;
	if (ActorToCreate != nullptr)
	{
		carryOn = GetWorld()->SpawnActor<AActor>(ActorToCreate, FVector::ZeroVector, FRotator::ZeroRotator);
		UpdateCarryOn();
	}
}

#endif 