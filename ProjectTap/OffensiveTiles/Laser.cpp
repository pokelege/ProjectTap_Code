// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Laser.h"
#include "../Pawns/BallPawn.h"
#include "../Pawns/TurretPawn.h"
#include "../Tiles/DeflectiveTile.h"
#include "../Tiles/BlockingTile.h"
#include "../Tiles/PortalTile.h"
#include "Classes/Particles/ParticleEmitter.h"

#define printonscreen(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)
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
	laserParticle->EmitterInstances[0]->SetBeamTargetPoint(GetActorLocation(), 0);
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
		currHitPoint = hit.ImpactPoint;

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
			bool isFrame = false;
			if (tile != nullptr)
			{
				typeFound = true;
				TArray<USceneComponent*> Children;
				tile->frameCollisionsComponent->GetChildrenComponents(true, Children);
				for(int i = 0; i < Children.Num() && typeFound ; ++i)
				{
//					printonscreen(hit.Component.Get()->GetName());
//					printonscreen(Children[i]->GetName());
					if(hit.Component.Get() == Children[i])
					{
						typeFound = false;
						isFrame = true;
					}
				}
				//cut the laser length to make sure new sub laser start doesn't hit the same object
				if(typeFound && CanSpawnSubLaser()) SpawnSubLaser(hit.ImpactPoint, hit.ImpactNormal);
			}


			//if sub laser already exists then keep updating its rotation and position
			auto subLaserExistsHitDeflectiveTile = currentDepth < MAX_DEPTH && nextLaser != nullptr && tile != nullptr;
			if (subLaserExistsHitDeflectiveTile)
			{
				auto incomingVector = hit.ImpactPoint - GetActorLocation();
				auto newDir = FMath::GetReflectionVector(incomingVector, hit.ImpactNormal);

				auto start = hit.ImpactPoint + newDir * 2.0f;
				nextLaser->SetActorLocation(hit.ImpactPoint);
				nextLaser->dir = newDir.IsNormalized() ? newDir : newDir.GetSafeNormal();
				nextLaser->laserParticle->EmitterInstances[0]->SetBeamSourcePoint(hit.ImpactPoint, 0);
				nextLaser->laserParticle->EmitterInstances[0]->SetBeamTargetPoint(start + newDir * length, 0);
			}

			//if the laser hits blocking tile then make it go down faster
			if (!typeFound)
			{
				auto blockingTile = Cast<ABlockingTile>(hitActor);
				if (blockingTile != nullptr)
				{
					typeFound = true;
					blockingTile->ApplyActivationTimeFactor(0.5f);
				}

			}

			//if the laser hits turret then kills it
			if (!typeFound)
			{
				auto turret = Cast<ATurretPawn>(hitActor);
				if (turret != nullptr)
				{
					typeFound = true;
					turret->Damage(2.0f);
				}
			}

			APortalTile* portal = nullptr;
			if (!typeFound)
			{
				portal = Cast<APortalTile>(hitActor);
				if (CanSpawnSubLaser() && portal != nullptr)
				{
					typeFound = true;
					auto relativePos = hit.ImpactPoint - hit.GetComponent()->GetComponentLocation();
					currHitPoint = relativePos + portal->GetActorLocation();
					SpawnSubLaser(currHitPoint, hit.ImpactNormal);
				}
			}

			auto subLaserExistsHitPortalTile = currentDepth < MAX_DEPTH && nextLaser != nullptr && portal != nullptr;
			if (subLaserExistsHitPortalTile)
			{
				FVector newSourcePos;
				portal->GetLaserPortalTransportedLocation(hit.GetComponent(), nextLaser->dir, newSourcePos);
				nextLaser->SetActorLocation(newSourcePos);
				nextLaser->laserParticle->EmitterInstances[0]->SetBeamSourcePoint(newSourcePos, 0);
				nextLaser->laserParticle->EmitterInstances[0]->SetBeamTargetPoint(newSourcePos + nextLaser->dir * length, 0);
			}

			bool notHitDeflectiveTile = tile == nullptr || isFrame;
			bool notHitPortal = portal == nullptr;
			if (notHitDeflectiveTile && notHitPortal)
			{
				KillSubLaser();
			}
		}
	}
	else
	{
		currHitPoint = laserVector;

		laserEmitter->SetBeamTargetPoint(pos + currHitPoint, 0);
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
	nextLaser->laserParticle->EmitterInstances[0]->SetBeamTargetPoint(start + newDir * length, 0);
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
		laserParticle->EmitterInstances[0]->SetBeamSourcePoint(GetActorLocation(), 0);
		laserParticle->EmitterInstances[0]->SetBeamTargetPoint(GetActorLocation(), 0);;
		nextLaser->KillSubLaser();
		nextLaser->Destroy();
		nextLaser = nullptr;
	}
}


void ALaser::SetLaserLocationWithDefaultHitLocation(const FVector& location)
{

	SetActorLocation(location);
	laserParticle->EmitterInstances[0]->SetBeamSourcePoint(location, 0);
	laserParticle->EmitterInstances[0]->SetBeamTargetPoint(currHitPoint, 0);
}


void ALaser::SetLaserLocationWithDefaultDirection(const FVector& location)
{
	SetActorLocation(location);
	laserParticle->EmitterInstances[0]->SetBeamSourcePoint(location, 0);
}

