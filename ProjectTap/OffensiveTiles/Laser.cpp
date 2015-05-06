// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Laser.h"
#include "../Pawns/BallPawn.h"
#include "../Tiles/DeflectiveTile.h"
#include "../Tiles/BlockingTile.h"
#include "../Pawns/TurretPawn.h"
#include "Classes/Particles/ParticleEmitter.h"

// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<UEmptyComponent>(TEXT("RootEmpty"));
	SetRootComponent(root);
	laserParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserParticle"));
	laserParticle->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<UParticleSystem> particleAssets(TEXT("/Game/Particles/P_NewLaser"));
	laserParticle->SetTemplate(particleAssets.Object);

	debugArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("arrow"));
	debugArrow->AttachTo(RootComponent);
	/*laserParticle->EmitterInstances[0]->SetBeamSourcePoint(GetActorLocation(), 0);
	laserParticle->EmitterInstances[0]->SetBeamTargetPoint(GetActorLocation(), 0);*/
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("laser emitter"));
	mesh->AttachTo(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> laserEmitterMesh(TEXT("/Game/Models/TurretGun"));
	mesh->SetStaticMesh(laserEmitterMesh.Object);

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
	queryParam.bReturnFaceIndex = true;
	FCollisionObjectQueryParams objectParam = objectParam.DefaultObjectQueryParam;
	
	auto pos = GetActorLocation();
	auto rayStart = pos + dir * 2.0f;
	auto laserVector = dir * length;
	auto laserEmitter = laserParticle->EmitterInstances[0];

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

			bool typeFound = false;
			//if hits deflective tile then spawn a new laser object
			auto tile = Cast<ADeflectiveTile>(hitActor);
			if (CanSpawnSubLaser() && tile != nullptr && hit.Component != tile->frameMeshComponent)
			{
				typeFound = true;
				//cut the laser length to make sure new sub laser start doesn't hit the same object
				SpawnSubLaser(hit.ImpactPoint, hit.ImpactNormal);
			}
			else if(tile == nullptr || hit.Component == tile->frameMeshComponent)
			{
				KillSubLaser();
			}

			if (!typeFound)
			{
				auto blockingTile = Cast<ABlockingTile>(hitActor);
				if (blockingTile != nullptr)
				{
					typeFound = true;
					blockingTile->ApplyActivationTimeFactor(0.4f);
				}

			} 

			if (!typeFound)
			{
				auto turret = Cast<ATurretPawn>(hitActor);
				if (turret != nullptr)
				{
					typeFound = true;
				}

			}

			//if sub laser already exists then keep updating its rotation and position
			auto subLaserExists = currentDepth < MAX_DEPTH && nextLaser != nullptr && tile != nullptr;
			if (subLaserExists)
			{
				auto incomingVector = hit.ImpactPoint - GetActorLocation();
				auto newDir = FMath::GetReflectionVector(incomingVector, hit.ImpactNormal);

				auto start = hit.ImpactPoint + newDir * 2.0f;
				nextLaser->SetActorLocation(hit.ImpactPoint);
				nextLaser->dir = newDir.IsNormalized() ? newDir : newDir.GetSafeNormal();
				nextLaser->laserParticle->EmitterInstances[0]->SetBeamSourcePoint(hit.ImpactPoint, 0);
				nextLaser->laserParticle->EmitterInstances[0]->SetBeamTargetPoint(start + newDir * length, 0);
			}			
		}
	}
	else
	{
		laserEmitter->SetBeamTargetPoint(pos + laserVector, 0);
		KillSubLaser();
	}

	//only root laser can have an emitter mesh
	if (currentDepth == 0)
	{
		//update laser emitter rotation
		mesh->SetWorldScale3D(FVector(10.0f));
		mesh->SetWorldRotation(dir.Rotation());
	}

}

void ALaser::SpawnSubLaser(const FVector& start, const FVector& normal)
{
	auto incomingVector = start - GetActorLocation();
	auto newDir = FMath::GetReflectionVector(incomingVector, normal).GetSafeNormal();

	nextLaser = GetWorld()->SpawnActor<ALaser>();
	nextLaser->SetActorLocation(start);
	nextLaser->SetLaserDepth(currentDepth + 1);
	nextLaser->mesh->SetHiddenInGame(true);
	nextLaser->dir = newDir;
	nextLaser->laserParticle->EmitterInstances[0]->SetBeamSourcePoint(start, 0);
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


