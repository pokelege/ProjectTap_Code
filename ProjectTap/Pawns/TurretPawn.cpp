// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Engine/GameInstance.h"
#include "BallPawn.h"
#include "ProjectTapGameState.h"
#include "General/Bullet.h"
#include "TurretPawn.h"

const FName ATurretPawn::MESH = FName("/Game/Models/Turret");

// Sets default values
ATurretPawn::ATurretPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*MESH.ToString());
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Turret mesh" ) );
	TurretMesh->SetStaticMesh(mesh.Object);
	this->SetRootComponent(TurretMesh);
	//nozzle->SetWorldTransform();
}

// Called when the game starts or when spawned
void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
	nozzleLocal = TurretMesh->GetSocketLocation("Nozzle");
}

// Called every frame
void ATurretPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	currentFireCooldown += DeltaTime;
	currentUpdateCooldown += DeltaTime;
	if(currentUpdateCooldown < updateInterval) return;
	currentUpdateCooldown = 0;
	if(currentFireCooldown < fireRate ) return;
	FVector forward;

	forward = this->GetActorForwardVector();

	UGameInstance* gameInstance = GetGameInstance();
	FWorldContext* worldContext = gameInstance->GetWorldContext();
	UWorld* world = worldContext->World();
	AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
	ABallPawn* player = gameState->CurrentPawn;
	if(player == nullptr) return;
	FVector turretToBallNormal = (player->GetTransform().GetTranslation() - nozzleLocal);
	float distance = turretToBallNormal.Size();
	turretToBallNormal.Normalize();

	float dot = FVector::DotProduct(turretToBallNormal,forward);
	float radians = FMath::Cos(FMath::DegreesToRadians(FOV));
	if(dot < radians || distance > maxDistance) return;
	FRotator rotation;
	ABullet* bullet = this->GetWorld()->SpawnActor<ABullet>(nozzleLocal, rotation);

	UPrimitiveComponent* comp = Cast<UPrimitiveComponent>(bullet->GetRootComponent());
	comp->AddImpulse(turretToBallNormal * bulletForce);
// 	FHitResult hit(ForceInit);
// 	FCollisionQueryParams p = FCollisionQueryParams(FName(TEXT("Tracing")), true, this);
// 	p.bTraceComplex = true;
// 	p.bTraceAsyncScene = true;
// 	p.bReturnPhysicalMaterial = false;
//
// 	FCollisionResponseParams p2;
	//player->Kill();
	//world->LineTraceSingle(hit,translate, maxDistance * forward,ECollisionChannel::ECC_Pawn,p);
	//todo if actually found player
	//if(hit.GetActor() == player)
 	//{
		//todo shoot
		//player->Kill();
 	//}
	currentFireCooldown = 0;
}

// Called to bind functionality to input
void ATurretPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

