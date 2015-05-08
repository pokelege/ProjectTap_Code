// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Pawns/BallPawn.h"
#include "MagnetTile.h"


AMagnetTile::AMagnetTile() : ATile()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*FName("/Game/Models/Magnet").ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	BoxCollision->SetBoxExtent(FVector(1.0f));
	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(40.0f, 40.0f, 40.0f));
}

void AMagnetTile::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
	FHitResult hit;
	FCollisionQueryParams queryParam;
	queryParam.bFindInitialOverlaps = false;
	queryParam.bReturnFaceIndex = true;
	FCollisionObjectQueryParams objectParam = objectParam.DefaultObjectQueryParam;

	//todo use socket instead
	auto pos = TileMesh->GetSocketLocation("Socket");
	auto rayStart = pos + GetActorForwardVector() * 2.0f;
	auto laserVector = GetActorForwardVector() * length;
	//auto laserEmitter = laserParticle->EmitterInstances[0];

	GetWorld()->LineTraceSingle(hit,rayStart, pos + laserVector, queryParam, objectParam);
	auto hitActor = hit.Actor.Get();
	ABallPawn* pawn;
	if((pawn = Cast<ABallPawn>(hitActor)) != nullptr)
	{
		pawn->AddVelocity(-GetActorForwardVector() * 1000.0f, GetActorLocation(), false);
	}
}
