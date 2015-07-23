// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MagnetTile.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameState.h"
#include "Engine/GameInstance.h"
#include "Particles/ParticleEmitter.h"
#include "Tiles/PortalTile.h"
#include "DeflectiveTile.h"


const int32 AMagnetTile::MAX_DEPTH = 8;

AMagnetTile::AMagnetTile() : ATile()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*FName("/Game/Models/Magnet").ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	TileMesh->SetMobility(EComponentMobility::Static);

	BoxCollision->SetBoxExtent(FVector(40.0f));

	ConstructorHelpers::FObjectFinder<UParticleSystem> particle(*FName("/Game/Particles/P_Magnet").ToString());
	magnetParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Magnet particle"));
	magnetParticle->SetTemplate(particle.Object);
	magnetParticle->AttachTo(GetRootComponent());
	magnetParticle->SetVisibility(false);

	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	delegate.BindUFunction(this, TEXT("OnBeginHit"));
	BoxCollision->OnComponentHit.Add(delegate);

	ConstructorHelpers::FObjectFinder<USoundWave> magnetSoundFile(TEXT("/Game/Sound/S_MagnetLoop"));
	magnetSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Magnet Sound"));
	magnetSound->SetSound(magnetSoundFile.Object);
	magnetSound->bAutoActivate = false;
	magnetSound->AttachTo(BoxCollision);
}

AMagnetTile::AMagnetTile(int32 depth)
{
	if (depth > 0)
	{
		ConstructorHelpers::FObjectFinder<UParticleSystem> particle(*FName("/Game/Particles/P_Magnet").ToString());
		magnetParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Magnet particle"));
		magnetParticle->SetTemplate(particle.Object);
		magnetParticle->AttachTo(GetRootComponent());
		magnetParticle->SetVisibility(false);

		auto pc = Cast<UPrimitiveComponent>(RootComponent);
		pc->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
		delegate.BindUFunction(this, TEXT("OnBeginHit"));
		BoxCollision->OnComponentHit.Add(delegate);

	}
}

void AMagnetTile::BeginPlay()
{
	Super::BeginPlay();
	if (IsMaster())
	{
		GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("ActivateCube", IE_Released, this, &AMagnetTile::deactivate);
	}
	magnetParticle->SetVisibility(true);
	magnetParticle->DeactivateSystem();
	magnetParticle->Deactivate();
}

OffsetInfo AMagnetTile::getOffsetInfo()
{
	OffsetInfo data;
	data.offsetForCollision = FVector(0.0f, 0.0f, 40.0f);
	data.scaleForCollision = FVector(1.0f, 1.0f, 5.0f);
	data.offsetForCarryOn = FVector(0.0f, 0.0f, 40.0f);
	return data;
}

void AMagnetTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!activated)return;
	
	FHitResult hit;
	auto hitActor = GetMagnetHitActor(hit);
	
	//update magnet particle 	
	magnetEndPos = hit.bBlockingHit ? hit.ImpactPoint : GetClampedForwardVector(true) + GetActorLocation();
	magnetParticle->EmitterInstances[0]->SetBeamTargetPoint(magnetEndPos, 0);
}

FVector AMagnetTile::GetClampedForwardVector(bool infiniteLength)
{
	auto vec = ADeflectiveTile::clampShortAxis(GetActorForwardVector(), true);

	return infiniteLength ? vec * length : vec;
}


void AMagnetTile::UpdateBallPawnSearch(AActor* actor)
{
	ABallPawn* pawn = Cast<ABallPawn>(actor);

	if ((pawn != nullptr) && !pawn->isDying())
	{
		PullBall(pawn);
	}
}

void AMagnetTile::UpdatePortalSearch(AActor* actor)
{
	auto portal = Cast<APortalTile>(actor);
	if (portal != nullptr)
	{
		auto 
	}
}

AActor* AMagnetTile::GetMagnetHitActor(FHitResult& hit)
{
	FCollisionQueryParams queryParam;
	queryParam.bFindInitialOverlaps = false;
	queryParam.bReturnFaceIndex = true;
	FCollisionObjectQueryParams objectParam;
	objectParam.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	objectParam.AddObjectTypesToQuery(ECC_Portal);

	auto pos = TileMesh->GetSocketLocation("Socket");

	auto rayStart = pos + GetClampedForwardVector() * 2.0f;
	auto laserVector = GetClampedForwardVector(true);

	GetWorld()->LineTraceSingleByObjectType(hit, rayStart, pos + laserVector, objectParam, queryParam);
	magnetEndPos = hit.ImpactPoint;
	return hit.Actor.Get();
}

void AMagnetTile::OnBeginHit(class AActor* OtherActor,
class UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	ABallPawn* ball = nullptr;
	if ((ball = Cast<ABallPawn>(OtherActor)) != nullptr)
	{
		ball->Kill();
		if (ball->isDying())
		{
			material->SetScalarParameterValue(TEXT("KilledBall"), 1);
		}
	}
}

void AMagnetTile::PullBall(ABallPawn* ball)
{
	auto prim = Cast<UPrimitiveComponent>(ball->GetRootComponent());
	UWorld* world = GetWorld();
	auto DeltaTime = world->DeltaTimeSeconds;
	AProjectTapGameState* gameState;
	if (world != nullptr && (gameState = world->GetGameState<AProjectTapGameState>()) != nullptr && gameState->SetMagnetTile(this) != this)
	{
		FVector linear = prim->GetPhysicsLinearVelocity();
		linear.X = 0;
		linear.Y = 0;
		FVector angular = prim->GetPhysicsAngularVelocity();
		angular.X = 0;
		angular.Y = 0;
		angular.Z = 0;
		prim->SetPhysicsLinearVelocity(linear);
		prim->SetPhysicsAngularVelocity(angular);
		float distanceAtNormal = FVector::DotProduct(ball->GetActorLocation() - GetActorLocation(), GetActorForwardVector());
		FVector normalLoc = (distanceAtNormal * GetActorForwardVector()) + GetActorLocation();
		FVector normalToBall = ball->GetActorLocation() - normalLoc;
		float dist = normalToBall.Size();
		if (dist > centerTolerance)
		{
			FVector toAdd = dist * -normalToBall.GetSafeNormal();
			toAdd.Z = 0;
			prim->AddRelativeLocation(toAdd);
		}
	}
	if (isVertical)
	{
		targetVelocity *= verticalForceMultiplier;
	}

	prim->AddImpulse(targetVelocity * -GetActorForwardVector());
}

void AMagnetTile::deactivate()
{
	if (!activated) return;
	Super::deactivate();

	magnetParticle->DeactivateSystem();
	magnetParticle->Deactivate();
	magnetSound->Stop();
}

void AMagnetTile::activate()
{
	if (activated) return;
	Super::activate();
	magnetParticle->Activate(true);
	magnetParticle->ActivateSystem();
	magnetParticle->EmitterInstances[0]->SetBeamTargetPoint(magnetParticle->GetComponentLocation(), 0);
	magnetSound->Play();
}

bool AMagnetTile::IsMaster()
{
	return currentDepth == 0;
}

void AMagnetTile::SpawnSubMagnet(const FVector& start,
	const FVector& end)
{
	
}

void AMagnetTile::KillSubMagnet()
{
	if (subMagnet != nullptr)
	{
		subMagnet->KillSubMagnet();
		Destroy();
	}
}

bool AMagnetTile::CanSpawnSubMagnet()
{

	return subMagnet == nullptr && currentDepth < MAX_DEPTH;
}
