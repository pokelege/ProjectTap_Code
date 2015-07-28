// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Laser.h"
#include "EmptyComponent.h"
#include "Pawns/BallPawn.h"
#include "Pawns/TurretPawn.h"
#include "Tiles/DeflectiveTile.h"
#include "Tiles/BlockingTile.h"
#include "Tiles/PortalTile.h"
#include "Classes/Particles/ParticleEmitter.h"
#include "ProjectTapGameState.h"
#include "Engine/GameInstance.h"

#define printonscreen(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)
const int ALaser::MAX_DEPTH = 8;
const GroundableInfo ALaser::groundableInfo = GroundableInfo(FVector(0,0,40), true);
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

	laserSparkParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserSparkParticle"));
	laserSparkParticle->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<UParticleSystem> particleSparkAssets(TEXT("/Game/Particles/P_LaserSparks"));
	laserSparkParticle->SetTemplate(particleSparkAssets.Object);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("laser emitter"));
	mesh->AttachTo(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> laserEmitterMesh(TEXT("/Game/Models/TurretGun"));
	mesh->SetStaticMesh(laserEmitterMesh.Object);
	mesh->SetCastShadow(false);

	mesh->SetWorldScale3D(FVector(0.3f));

	ConstructorHelpers::FObjectFinder<USoundWave> laserEmitSoundFile( TEXT( "/Game/Sound/S_LaserLoop" ) );
	laserEmitSound = CreateDefaultSubobject<UAudioComponent>( TEXT( "Laser Emit Sound" ) );
	laserEmitSound->SetSound( laserEmitSoundFile.Object );
	laserEmitSound->AttachTo( GetRootComponent() );
}

const GroundableInfo* ALaser::GetGroundableInfo() const
{
	return &ALaser::groundableInfo;
}

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();
	UWorld* world = GetWorld();
	AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
	OnGameStateChangedDelegateHandle = gameState->GameStateChanged.AddUFunction( this , TEXT( "OnStateChanged" ) );
	laserParticle->EmitterInstances[0]->SetBeamSourcePoint(GetActorLocation(), 0);
	laserParticle->EmitterInstances[0]->SetBeamTargetPoint(GetActorLocation(), 0);
	laserEmitSound->Play();
}
void ALaser::BeginDestroy()
{
	UWorld* world = GetWorld();
	AProjectTapGameState* gameState;
	if ( world != nullptr && ( gameState = world->GetGameState<AProjectTapGameState>() ) != nullptr )
	{
		gameState->GameStateChanged.Remove( OnGameStateChangedDelegateHandle );
		OnGameStateChangedDelegateHandle.Reset();
	}
	Super::BeginDestroy();
}
void ALaser::OnStateChanged( const CustomGameState newState )
{
	canHitBall = newState == CustomGameState::GAME_STATE_PLAYING;
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
	queryParam.bReturnFaceIndex = true;
	FCollisionObjectQueryParams objectParam = objectParam.DefaultObjectQueryParam;
	if ( !canHitBall ) queryParam.AddIgnoredActor( GetWorld()->GetGameState<AProjectTapGameState>()->GetPlayer() );
	auto pos = GetActorLocation();
	auto rayStart = pos + dir * 5.0f;
	auto laserVector = dir * length;
	auto laserEmitter = laserParticle->EmitterInstances[0];
	//ray cast to see if laser hits anything
	GetWorld()->LineTraceSingleByObjectType(hit,rayStart, pos + laserVector, objectParam,queryParam);
	auto hitActor = hit.Actor.Get();

	if (hitActor != nullptr)
	{
		currHitPoint = hit.ImpactPoint;
		if(!laserSparkParticle->bIsActive) laserSparkParticle->ActivateSystem();
		//kills ball if laser hits it
		auto ball = Cast<ABallPawn>(hitActor);

		if (ball != nullptr)
		{
			ball->Kill();
			laserEmitter->SetBeamTargetPoint(hit.ImpactPoint, 0);
			KillSubLaser();
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
				if (typeFound && CanSpawnSubLaser()) SpawnSubLaser(hit.ImpactPoint + hit.ImpactNormal * 10.0f, hit.ImpactNormal);
			}


			//if sub laser already exists then keep updating its rotation and position
			auto subLaserExistsHitDeflectiveTile = currentDepth < MAX_DEPTH && nextLaser != nullptr && tile != nullptr;
			if (subLaserExistsHitDeflectiveTile)
			{
				auto incomingVector = hit.ImpactPoint - GetActorLocation();

				//if the incoming vector's angle is too small then kill sublasers to avoid laser flickering
				auto dot = FVector::DotProduct(-incomingVector.GetSafeNormal(), hit.ImpactNormal);

				auto angle = FMath::RadiansToDegrees(FMath::Acos(dot));
				if (angle < 70.0f)
				{
					auto newDir = FMath::GetReflectionVector(incomingVector, hit.ImpactNormal);

					auto start = hit.ImpactPoint + newDir * 2.0f;
					nextLaser->SetActorLocation(hit.ImpactPoint);
					nextLaser->dir = newDir.IsNormalized() ? newDir : newDir.GetSafeNormal();
					nextLaser->laserParticle->EmitterInstances[0]->SetBeamSourcePoint(hit.ImpactPoint, 0);
					nextLaser->laserParticle->EmitterInstances[0]->SetBeamTargetPoint(start + newDir * length, 0);
				}
				else
				{
					KillSubLaser();
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
			laserSparkParticle->SetWorldLocation(currHitPoint);
			laserSparkParticle->SetWorldRotation(FVector(currHitPoint -
												 GetActorLocation()).GetSafeNormal().Rotation().Quaternion());
	}
	else
	{
		currHitPoint = laserVector;
		laserEmitter->SetBeamTargetPoint(pos + currHitPoint, 0);
		laserSparkParticle->SetWorldLocation( pos + currHitPoint );
		laserSparkParticle->SetWorldRotation( FVector( (pos + currHitPoint) -
			GetActorLocation() ).GetSafeNormal().Rotation().Quaternion() );
		KillSubLaser();
	}

	//only root laser can have an emitter mesh
	if (currentDepth != 0)
	{
		//update laser emitter rotation
		mesh->SetVisibility(false);
	}
	else
	{
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
		//laserParticle->EmitterInstances[0]->SetBeamSourcePoint(GetActorLocation(), 0);
		//laserParticle->EmitterInstances[0]->SetBeamTargetPoint(GetActorLocation(), 0);;
		nextLaser->KillSubLaser();
		nextLaser->Destroy();
		nextLaser = nullptr;
	}
}

OffsetInfo ALaser::getOffsetInfo()
{
	OffsetInfo data;
	data.offsetForCollision = FVector(0.0f, 0.0f, 10.0f);
	data.scaleForCollision = FVector(1.0f, 1.0f, 5.0f);
	data.offsetForCarryOn = FVector(0.0f, 0.0f, 20.0f);
	return data;
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

