// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "TurretPawn.h"
#include "Engine/GameInstance.h"
#include "BallPawn.h"
#include "ProjectTapGameState.h"
#include "General/Bullet.h"
#include "ParticleEmitterInstances.h"

const FName ATurretPawn::BASE_MESH = FName( "/Game/Models/TurretBase" );
const FName ATurretPawn::GUN_MESH = FName( "/Game/Models/TurretGun" );
const float ATurretPawn::MAX_HEALTH = 10.0f;
// Sets default values
ATurretPawn::ATurretPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> baseMeshSource( *BASE_MESH.ToString() );

	UBoxComponent* collisionBox = CreateDefaultSubobject<UBoxComponent>( TEXT( "Turret Collision" ) );
	collisionBox->SetBoxExtent( FVector( 1 , 1 , 3 ) );
	this->SetRootComponent( collisionBox );
	collisionBox->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	collisionBox->bGenerateOverlapEvents = false;
	collisionBox->SetCollisionResponseToAllChannels( ECollisionResponse::ECR_Block );
	collisionBox->SetCollisionObjectType( ECollisionChannel::ECC_WorldDynamic );
	collisionBox->SetNotifyRigidBodyCollision( true );
	UStaticMeshComponent* baseMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Turret base mesh" ) );
	baseMesh->SetStaticMesh( baseMeshSource.Object );
	baseMesh->AttachTo( collisionBox );
	baseMesh->SetMobility( EComponentMobility::Static );

	ConstructorHelpers::FObjectFinder<UStaticMesh> gunMesh( *GUN_MESH.ToString() );
	TurretGunMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Turret gun mesh" ) );
	TurretGunMesh->SetStaticMesh( gunMesh.Object );
	TurretGunMesh->AttachTo( baseMesh );
	FTransform transform;
	transform.SetLocation( FVector( 0 , 0 , 2 ) );
	TurretGunMesh->SetRelativeTransform( transform );

	laserTag = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "Turret Laser Tag" ) );
	ConstructorHelpers::FObjectFinder<UParticleSystem> laserParticle( TEXT( "/Game/Particles/P_TurretLaser" ) );
	laserTag->SetTemplate( laserParticle.Object );
	laserTag->AttachTo( baseMesh );

	explosionParticle = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "Turret Explosion Tag" ) );
	ConstructorHelpers::FObjectFinder<UParticleSystem> explosionParticleTemplate( TEXT( "/Game/StarterContent/Particles/P_Explosion" ) );
	explosionParticle->SetTemplate( explosionParticleTemplate.Object );
	explosionParticle->AttachTo( TurretGunMesh );

	auto laserPrimitive = Cast<UPrimitiveComponent>( laserTag );
	FTransform laserWorldTransform = laserPrimitive->GetComponentTransform();

	auto explosionPrimitive = Cast<UPrimitiveComponent>( explosionParticle );
	FTransform explosionWorldTransform = explosionPrimitive->GetComponentTransform();

	ConstructorHelpers::FObjectFinder<USoundBase> explosionSoundFile( TEXT( "/Game/Sound/S_Explosion" ) );
	explosionSound = CreateDefaultSubobject<UAudioComponent>( TEXT( "Explosion Sound" ) );
	explosionSound->SetSound( explosionSoundFile.Object );
	explosionSound->bAutoActivate = false;
	explosionSound->AttachTo( explosionParticle );

	ConstructorHelpers::FObjectFinder<USoundWave> fireSoundFile( TEXT( "/Game/Sound/S_Gunshot" ) );
	fireSound = CreateDefaultSubobject<UAudioComponent>( TEXT( "Fire Sound" ) );
	fireSound->SetSound( fireSoundFile.Object );
	fireSound->bAutoActivate = false;
	fireSound->AttachTo( TurretGunMesh );

	auto pc = Cast<UPrimitiveComponent>( RootComponent );
	pc->SetWorldScale3D( FVector( 40.0f , 40.0f , 40.0f ) );
	laserPrimitive->SetWorldScale3D( laserWorldTransform.GetScale3D() );
	explosionPrimitive->SetWorldScale3D( explosionWorldTransform.GetScale3D() );
}

// Called when the game starts or when spawned
void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
	UWorld* world = GetWorld();
	AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
	OnPlayerChangedDelegateHandle = gameState->PlayerChanged.AddUFunction( this , TEXT( "OnPlayerChanged" ) );

	nozzleLocal = TurretGunMesh->GetSocketLocation( "Nozzle" );
	nozzleLocalUpdatable = TurretGunMesh->GetSocketLocation( "Nozzle" );
	laserTag->EmitterInstances[0]->SetBeamSourcePoint( nozzleLocal , 0 );
	fireSound->SetWorldLocation( nozzleLocal );
	direction = this->GetActorForwardVector();
	explosionParticle->Deactivate();
}

void ATurretPawn::OnPlayerChanged( ABallPawn* newPlayer )
{
	target = newPlayer;
}

bool ATurretPawn::FoundPlayerToHit()
{
	FVector forward;

	forward = TurretGunMesh->GetForwardVector();

	if ( target == nullptr ) return false;
	FVector turretToBallNormal = ( target->GetTransform().GetTranslation() - nozzleLocal ).GetSafeNormal();
	float distance = FVector::DistSquared( target->GetActorLocation() , nozzleLocal );

	float dot = FVector::DotProduct( turretToBallNormal , forward );
	float radians = FMath::Cos( FMath::DegreesToRadians( FOV ) );
	if ( dot < radians || distance > maxDistance * maxDistance ) return false;
	FHitResult hit;
	FCollisionQueryParams queryParam;
	queryParam.bFindInitialOverlaps = false;
	queryParam.bReturnFaceIndex = true;
	FCollisionObjectQueryParams objectParam = objectParam.DefaultObjectQueryParam;

	auto pos = TurretGunMesh->GetSocketLocation( "Nozzle" );
	auto rayStart = pos + ( target->GetActorLocation() - nozzleLocalUpdatable ).GetSafeNormal();
	auto laserVector = ( target->GetActorLocation() - nozzleLocalUpdatable ).GetSafeNormal() * maxDistance;

	GetWorld()->LineTraceSingleByObjectType( hit , rayStart , pos + laserVector , objectParam , queryParam );
	while ( hit.GetActor() != nullptr && Cast<ABullet>( hit.GetActor() ) != nullptr )
	{
		queryParam.AddIgnoredComponent( hit.GetComponent() );
		hit = FHitResult();
		GetWorld()->LineTraceSingleByObjectType( hit , rayStart , pos + laserVector , objectParam , queryParam );
	}
	return Cast<ABallPawn>( hit.GetActor() ) != nullptr;
}

void ATurretPawn::Fire()
{
	if ( target == nullptr ) return;

	ABullet* bullet = this->GetWorld()->SpawnActor<ABullet>( nozzleLocalUpdatable , TurretGunMesh->GetForwardVector().Rotation() );

	UPrimitiveComponent* comp = Cast<UPrimitiveComponent>( bullet->GetRootComponent() );
	comp->AddImpulse( TurretGunMesh->GetForwardVector() * bulletForce );
	fireSound->Play();
}


void ATurretPawn::AttemptToFire( const float& DeltaTime )
{

	if ( currentFireCooldown < fireRate ) return;

	Fire();
	currentFireCooldown = 0;
}

// Called every frame
void ATurretPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	nozzleLocalUpdatable = TurretGunMesh->GetSocketLocation( "Nozzle" );
	if ( !activated || died )
	{
		laserTag->EmitterInstances[0]->SetBeamSourcePoint( nozzleLocalUpdatable , 0 );
		laserTag->EmitterInstances[0]->SetBeamTargetPoint( nozzleLocalUpdatable , 0 );
		return;
	}
	currentFireCooldown += DeltaTime;
	laserTag->EmitterInstances[0]->SetBeamSourcePoint( nozzleLocalUpdatable , 0 );
	fireSound->SetWorldLocation( nozzleLocalUpdatable );

	if ( FoundPlayerToHit() )
	{
		auto targetVector = ( target->GetActorLocation() - TurretGunMesh->GetComponentLocation() ).GetSafeNormal();
		auto targetRotation = targetVector.Rotation();
		if ( FMath::Abs<float>( targetRotation.Yaw ) <= rotation )
		{
			TurretGunMesh->SetRelativeRotation( FMath::RInterpTo( TurretGunMesh->GetRelativeTransform().Rotator() , targetRotation , DeltaTime , ballSightedRotateSpeed ) );
			auto dot = FVector::DotProduct( TurretGunMesh->GetForwardVector() , targetVector );
			if ( dot > 0 && 1.0f - dot < maxErrorToShoot )
				AttemptToFire( DeltaTime );
		}
	}
	else
	{
		currentTime += ( DeltaTime * idleRotateSpeed );
		regularRotation = FRotator( 0 , FMath::Sin( currentTime ) * ( rotation * 0.5f ) , 0 );
		TurretGunMesh->SetRelativeRotation( FMath::RInterpTo( TurretGunMesh->GetRelativeTransform().Rotator() , regularRotation , DeltaTime , idleRotateSpeed ) );
		nozzleLocalUpdatable = TurretGunMesh->GetSocketLocation( "Nozzle" );
	}
	UpdateLaserTag( DeltaTime );
}

// Called to bind functionality to input
void ATurretPawn::SetupPlayerInputComponent( class UInputComponent* InputComponent )
{
	Super::SetupPlayerInputComponent( InputComponent );
}

void ATurretPawn::UpdateLaserTag( float dt )
{
	auto l = target->GetActorLocation();
	direction = TurretGunMesh->GetForwardVector();

	FCollisionQueryParams queryParams;
	queryParams.bFindInitialOverlaps = true;
	FCollisionObjectQueryParams objParams;
	//change laser length
	FHitResult hit;
	auto end = nozzleLocalUpdatable + direction * maxDistance;
	GetWorld()->LineTraceSingleByObjectType( hit , nozzleLocalUpdatable , end , objParams );

	auto laserLength = maxDistance;

	if ( hit.Actor != nullptr )
	{
		laserLength = ( hit.Actor.Get()->GetActorLocation() - nozzleLocalUpdatable ).Size();
	}
	laserTag->EmitterInstances[0]->SetBeamSourcePoint( nozzleLocalUpdatable , 0 );
	laserTag->EmitterInstances[0]->SetBeamTargetPoint( nozzleLocalUpdatable + direction * laserLength , 0 );
}

void ATurretPawn::Damage( float deathDuration )
{
	if ( deathDuration == 0.0f )
	{
		Kill();
	}
	else
	{
		auto damage = MAX_HEALTH / deathDuration;
		current_hp -= damage;

		if ( current_hp < 0.0f )
		{
			Kill();
		}
	}

}

void ATurretPawn::Kill()
{
	if ( died )return;
	died = true;
	explosionParticle->Activate( true );
	TurretGunMesh->SetStaticMesh( nullptr );
	explosionSound->Activate();
	explosionSound->Play();
}
