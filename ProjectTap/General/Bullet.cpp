// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Bullet.h"
#include "Pawns/BallPawn.h"
#include "Pawns/TurretPawn.h"

const FName ABullet::MESH = FName( "/Game/Models/Bullet" );
// Sets default values
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh( *MESH.ToString() );
	UStaticMeshComponent* BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Bullet mesh" ) );
	BulletMesh->SetStaticMesh( mesh.Object );
	this->SetRootComponent( BulletMesh );
	BulletMesh->SetNotifyRigidBodyCollision(true);
	BulletMesh->SetSimulatePhysics(true);
	BulletMesh->SetEnableGravity(false);
	delegate.BindUFunction(this, TEXT("OnBeginHit"));
	BulletMesh->OnComponentHit.Add(delegate);
}

void ABullet::OnBeginHit(class AActor* OtherActor,
				class UPrimitiveComponent* OtherComp,
				FVector NormalImpulse,
				const FHitResult& Hit)
{
	if(Cast<ATurretPawn>(OtherActor) != nullptr) return;
	ABallPawn* ball = nullptr;
	if ( ( ball = Cast<ABallPawn>( OtherActor ) ) != nullptr )
	{
		ball->Kill();
	}
	Destroy();
}
