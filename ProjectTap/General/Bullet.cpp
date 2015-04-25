// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Pawns/BallPawn.h"
#include "Pawns/TurretPawn.h"
#include "Bullet.h"

const FName ABullet::MESH = FName( "/Game/Models/Bullet" );
// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh( *MESH.ToString() );
	UStaticMeshComponent* BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Bullet mesh" ) );
	BulletMesh->SetStaticMesh( mesh.Object );
	this->SetRootComponent( BulletMesh );
	BulletMesh->SetNotifyRigidBodyCollision(true);
	BulletMesh->SetSimulatePhysics(true);
	BulletMesh->SetEnableGravity(false);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABullet::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ABullet::ReceiveHit
(
    class UPrimitiveComponent * MyComp,
    AActor * Other,
    class UPrimitiveComponent * OtherComp,
    bool bSelfMoved,
    FVector HitLocation,
    FVector HitNormal,
    FVector NormalImpulse,
    const FHitResult & Hit
)
{
	if(Cast<ATurretPawn>(Other) != nullptr) return;
	ABallPawn* ball = nullptr;
	if ( ( ball = Cast<ABallPawn>( Other ) ) != nullptr )
	{
		ball->Kill();
	}
	Destroy();
}
