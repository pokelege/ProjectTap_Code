// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Engine/GameInstance.h"
#include "BallPawn.h"
#include "ProjectTapGameState.h"
#include "General/Bullet.h"
#include "TurretPawn.h"
#include "ParticleEmitterInstances.h"

const FName ATurretPawn::BASE_MESH = FName("/Game/Models/TurretBase");
const FName ATurretPawn::GUN_MESH = FName("/Game/Models/TurretGun");

// Sets default values
ATurretPawn::ATurretPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> baseMeshSource(*BASE_MESH.ToString());

	UStaticMeshComponent* baseMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Turret base mesh" ) );
	baseMesh->SetStaticMesh(baseMeshSource.Object);
	this->SetRootComponent(baseMesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh> gunMesh(*GUN_MESH.ToString());
	TurretGunMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Turret gun mesh" ) );
	TurretGunMesh->SetStaticMesh(gunMesh.Object);
	TurretGunMesh->AttachTo(baseMesh);
	FTransform transform;
	transform.SetLocation(FVector(0,0,2));
	TurretGunMesh->SetRelativeTransform(transform);

	laserTag = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Turret Laser Tag"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> laserParticle(TEXT("/Game/Particles/P_TurretLaser"));
	laserTag->SetTemplate(laserParticle.Object);
	laserTag->AttachTo(baseMesh);

	auto laserPrimitive = Cast<UPrimitiveComponent>(laserTag);
	FTransform laserWorldTransform = laserPrimitive->GetComponentTransform();

	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(40.0f, 40.0f, 40.0f));
	laserPrimitive->SetWorldScale3D(laserWorldTransform.GetScale3D());
}

// Called when the game starts or when spawned
void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
	nozzleLocal = TurretGunMesh->GetSocketLocation("Nozzle");
	laserTag->EmitterInstances[0]->SetBeamSourcePoint(nozzleLocal, 0);
}

bool ATurretPawn::FoundPlayerToHit()
{
	FVector forward;

	forward = this->GetActorForwardVector();

	AProjectTapGameState* gameState = GetWorld()->GetGameState<AProjectTapGameState>();
	ABallPawn* player = gameState->CurrentPawn;
	if(player == nullptr) return false;
	FVector turretToBallNormal = (player->GetTransform().GetTranslation() - nozzleLocal);
	float distance = turretToBallNormal.Size();
	turretToBallNormal.Normalize();

	float dot = FVector::DotProduct(turretToBallNormal,forward);
	float radians = FMath::Cos(FMath::DegreesToRadians(FOV));
	if(dot < radians || distance > maxDistance) return false;
	return true;
}

void ATurretPawn::Fire()
{
	AProjectTapGameState* gameState = GetWorld()->GetGameState<AProjectTapGameState>();
	ABallPawn* player = gameState->CurrentPawn;
	if(player == nullptr) return;
	FVector turretToBallNormal = (player->GetTransform().GetTranslation() - nozzleLocal);
	float distance = turretToBallNormal.Size();
	turretToBallNormal.Normalize();

	FRotator rotation;
	ABullet* bullet = this->GetWorld()->SpawnActor<ABullet>(nozzleLocal, rotation);

	UPrimitiveComponent* comp = Cast<UPrimitiveComponent>(bullet->GetRootComponent());
	comp->AddImpulse(turretToBallNormal * bulletForce);
}


void ATurretPawn::AttemptToFire(const float& DeltaTime)
{

	currentUpdateCooldown += DeltaTime;

	if(currentFireCooldown < fireRate ) return;

	if(!FoundPlayerToHit()) return;
	Fire();
	currentFireCooldown = 0;
}

// Called every frame
void ATurretPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if(!activated) return;
	UpdateLaserTag(DeltaTime);

	currentFireCooldown += DeltaTime;
	AttemptToFire(DeltaTime);
	if(currentUpdateCooldown < updateInterval) return;
	currentUpdateCooldown = 0;
}

// Called to bind functionality to input
void ATurretPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}


void ATurretPawn::UpdateLaserTag(float dt)
{
	auto state  = GetWorld()->GetGameState<AProjectTapGameState>();
	auto pawn = state->CurrentPawn;
	
	auto l = pawn->GetActorLocation();
	direction = (pawn->GetActorLocation() - nozzleLocal).GetSafeNormal();
	laserTag->EmitterInstances[0]->SetBeamTargetPoint(nozzleLocal + direction * maxDistance,0);

	auto gunPrimitive = Cast<UPrimitiveComponent>(TurretGunMesh);
	gunPrimitive->SetWorldRotation(direction.Rotation());
}

