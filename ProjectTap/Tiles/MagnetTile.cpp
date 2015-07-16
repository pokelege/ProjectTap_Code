// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "MagnetTile.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameState.h"
#include "Engine/GameInstance.h"

AMagnetTile::AMagnetTile() : ATile()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*FName("/Game/Models/Magnet").ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	TileMesh->SetMobility( EComponentMobility::Static );

	BoxCollision->SetBoxExtent(FVector(40.0f));

	ConstructorHelpers::FObjectFinder<UParticleSystem> particle(*FName("/Game/Particles/P_Magnet").ToString());
	magnetParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Magnet particle"));
	magnetParticle->SetTemplate(particle.Object);
	magnetParticle->AttachTo(GetRootComponent());

	ConstructorHelpers::FObjectFinder<UStaticMesh> distortion(*FName("/Game/Models/distortion").ToString());
	distortionMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Distortion mesh" ) );
	distortionMesh->SetStaticMesh(distortion.Object);
	distortionMesh->SetWorldScale3D(FVector(0,1,1));
	distortionMesh->AttachTo(RootComponent);
	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	delegate.BindUFunction( this , TEXT( "OnBeginHit" ) );
	BoxCollision->OnComponentHit.Add( delegate );

	ConstructorHelpers::FObjectFinder<USoundWave> magnetSoundFile( TEXT( "/Game/Sound/S_MagnetLoop" ) );
	magnetSound = CreateDefaultSubobject<UAudioComponent>( TEXT( "Magnet Sound" ) );
	magnetSound->SetSound( magnetSoundFile.Object );
	magnetSound->bAutoActivate = false;
	magnetSound->AttachTo( BoxCollision );
}

void AMagnetTile::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("ActivateCube", IE_Released, this, &AMagnetTile::deactivate);
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

void AMagnetTile::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
	if ( !activated )return;
	ABallPawn* pawn = FindBallPawn();
	if((pawn != nullptr))
	{
		PullBall(pawn, DeltaTime);
	}
}

void AMagnetTile::OnBeginHit( class AActor* OtherActor ,
class UPrimitiveComponent* OtherComp ,
	FVector NormalImpulse ,
	const FHitResult& Hit )
{
	ABallPawn* ball = nullptr;
	if ( ( ball = Cast<ABallPawn>( OtherActor ) ) != nullptr )
	{
		ball->Kill();
		if ( ball->isDying() )
		{
			material->SetScalarParameterValue( TEXT( "KilledBall" ) , 1 );
		}
	}
}

class ABallPawn* AMagnetTile::FindBallPawn()
{
	FHitResult hit;
	FCollisionQueryParams queryParam;
	queryParam.bFindInitialOverlaps = false;
	queryParam.bReturnFaceIndex = true;
	FCollisionObjectQueryParams objectParam;
	objectParam.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	auto pos = TileMesh->GetSocketLocation("Socket");
	auto rayStart = pos + GetActorForwardVector() * 2.0f;
	auto laserVector = GetActorForwardVector() * length;

	GetWorld()->LineTraceSingleByObjectType(hit,rayStart, pos + laserVector, objectParam,queryParam);
	auto hitActor = hit.Actor.Get();
	return Cast<ABallPawn>(hitActor);
}

void AMagnetTile::PullBall(class ABallPawn* ball, float DeltaTime)
{
	auto prim = Cast<UPrimitiveComponent>(ball->GetRootComponent());
	UWorld* world = GetWorld();
	AProjectTapGameState* gameState;
	if ( world != nullptr && ( gameState = world->GetGameState<AProjectTapGameState>() ) != nullptr && gameState->SetMagnetTile( this ) != this)
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
		if(dist > centerTolerance)
		{
			FVector toAdd = dist * -normalToBall.GetSafeNormal();
			toAdd.Z = 0;
			prim->AddRelativeLocation(toAdd);
		}
	}
	prim->AddImpulse(targetVelocity * -GetActorForwardVector());
}

 void AMagnetTile::deactivate()
 {
	 if(!activated) return;
	 Super::deactivate();

	 magnetParticle->DeactivateSystem();
	 magnetParticle->Deactivate();
	 distortionMesh->SetRelativeScale3D(FVector(0,1,1));
	 magnetSound->Stop();
 }

 void AMagnetTile::activate()
 {
	 Super::activate();
	 magnetParticle->Activate(true);
	 magnetParticle->ActivateSystem();
	 distortionMesh->SetRelativeScale3D(FVector(length,1,1));
	 magnetSound->Play();
 }
