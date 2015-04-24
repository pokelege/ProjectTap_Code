// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Laser.h"
#include "ParticleEmitterInstances.h"
#include "../Pawns/BallPawn.h"

// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	laserParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserParticle"));
	SetRootComponent(laserParticle);

	ConstructorHelpers::FObjectFinder<UParticleSystem> particleAssets(TEXT("/Game/Particles/P_Laser"));
	particleAsset = particleAssets.Object;
	laserParticle->SetTemplate(particleAsset);
}

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();
	laserParticle->EmitterInstances[0]->SetBeamSourcePoint(GetActorLocation(), 0);
}

void ALaser::SetLaserDepth(unsigned i)
{
	currentDepth = i;
}

// Called every frame
void ALaser::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (timer <= elapseTime)
	{
		timer += DeltaTime;
	}
	else
	{
		checkLaserCollisions(DeltaTime);
	}
}



void ALaser::checkLaserCollisions(float dt)
{
	FHitResult hit;
	FCollisionQueryParams queryParam;
	queryParam.bFindInitialOverlaps = false;
	FCollisionObjectQueryParams objectParam = objectParam.DefaultObjectQueryParam;
	
	auto pos = GetActorLocation();
	auto rayStart = pos + GetActorForwardVector() * 10.0f;
	auto laserVector = GetActorForwardVector() * length;
	auto laserEmitter = laserParticle->EmitterInstances[0];

	GetWorld()->LineTraceSingle(hit,rayStart, pos + laserVector, queryParam, objectParam);
	

	laserEmitter->SetBeamSourcePoint(pos, 0);
	if (hit.Actor.Get() != nullptr)
	{
		auto ball = Cast<ABallPawn>(hit.Actor.Get());
		
		if (ball != nullptr)
		{
			ball->Kill();
		}
		else
		{

			laserEmitter->SetBeamTargetPoint(hit.ImpactPoint, 0);

			if (CanSpawnSubLaser())
			{
				SpawnSubLaser(hit.ImpactPoint, hit.ImpactNormal);
			}
		}
	}
	else
	{
		laserEmitter->SetBeamTargetPoint(pos + laserVector, 0);
	}

	//if (hit.Actor.Get()->StaticClass() == DeflectiveTile::StaticClass())

}

void ALaser::SpawnSubLaser(const FVector& start, const FVector& normal)
{
	auto incomingVector = start - GetActorLocation();
	auto newDir = reflect(incomingVector, normal);
	nextLaser = GetWorld()->SpawnActor<ALaser>(start, newDir.Rotation());
	nextLaser->SetLaserDepth(++currentDepth);
}

FVector ALaser::reflect(const FVector& v1, const FVector& v2)
{
	auto n = -v2;
	auto dot = FVector::DotProduct(v1, n);
	auto n2 = n * dot;
	return (v1 - 2 * n2).GetSafeNormal();
}

bool ALaser::CanSpawnSubLaser()
{
	return currentDepth < MAX_DEPTH && nextLaser == nullptr;
}


