// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Laser.h"
#include "ParticleEmitterInstances.h"
#include "../Pawns/BallPawn.h"
#include "../Tiles/DeflectiveTile.h"
// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	laserParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserParticle"));
	SetRootComponent(laserParticle);
	debugArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("arrow"));
	debugArrow->AttachTo(RootComponent);

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

#define printonscreen(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

void ALaser::checkLaserCollisions(float dt)
{
	FHitResult hit;
	FCollisionQueryParams queryParam;
	queryParam.bFindInitialOverlaps = false;
	FCollisionObjectQueryParams objectParam = objectParam.DefaultObjectQueryParam;
	
	auto pos = GetActorLocation();
	auto rayStart = pos + GetActorForwardVector() * 2.0f;
	auto laserVector = GetActorForwardVector() * length;
	auto laserEmitter = laserParticle->EmitterInstances[0];

	//laserEmitter->SetBeamSourcePoint(FVector(100.0f, 100.0f, 100.0f), 0);

	//ray cast to see if laser hits anything
	GetWorld()->LineTraceSingle(hit,rayStart, pos + laserVector, queryParam, objectParam);
	auto hitActor = hit.Actor.Get();
	
	if (hitActor != nullptr)
	{
		//kills ball if laser hits it
		auto ball = Cast<ABallPawn>(hitActor);
		
		if (ball != nullptr)
		{
			ball->Kill();
		}
		else
		{

			//if not set laser end point
			laserEmitter->SetBeamTargetPoint(hit.ImpactPoint, 0);

			//if hits deflective tile then spawn a new laser object
			auto tile = Cast<ADeflectiveTile>(hitActor);
			if (CanSpawnSubLaser() && tile != nullptr)
			{
				//cut the laser length to make sure new sub laser start doesn't hit the same object
				auto cutEndPoint = hit.ImpactPoint - GetActorForwardVector();
				SpawnSubLaser(hit.ImpactPoint, hit.ImpactNormal);
			}

			//if sub laser already exists then keep updating its rotation
			auto subLaserExists = currentDepth < MAX_DEPTH && nextLaser != nullptr;
			if (subLaserExists)
			{
				auto rotation = reflect(hit.ImpactPoint, hit.ImpactNormal).Rotation();
				nextLaser->SetActorRotation(rotation);
			}
		}
	}
	else
	{
		laserEmitter->SetBeamTargetPoint(pos + laserVector, 0);
		KillSubLaser();
	}


}

void ALaser::SpawnSubLaser(const FVector& start, const FVector& normal)
{
	auto incomingVector = start - GetActorLocation();
	auto newDir = FMath::GetReflectionVector(incomingVector, normal);

	nextLaser = GetWorld()->SpawnActor<ALaser>();
	nextLaser->SetActorLocation(start);
	nextLaser->SetLaserDepth(currentDepth + 1);
	nextLaser->laserParticle->EmitterInstances[0]->SetBeamTargetPoint(start + newDir * length , 0);
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


void ALaser::KillSubLaser()
{
	if (nextLaser != nullptr)
	{
		nextLaser->KillSubLaser();
		nextLaser->Destroy();
		nextLaser = nullptr;
	}
}


