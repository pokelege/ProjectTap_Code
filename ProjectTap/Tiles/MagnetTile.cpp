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

void AMagnetTile::BeginPlay()
{
	GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("ActivateCube", IE_Released, this, &AMagnetTile::deactivate);
}

void AMagnetTile::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
	if(!activated) return;

	ABallPawn* pawn = FindBallPawn();
	if((pawn != nullptr))
	{

	}
}

void AMagnetTile::ReceiveHit
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
	ABallPawn* ball = nullptr;
	if ( ( ball = Cast<ABallPawn>( Other ) ) != nullptr )
	{
		ball->Kill();
	}
}

class ABallPawn* AMagnetTile::FindBallPawn()
{
	FHitResult hit;
	FCollisionQueryParams queryParam;
	queryParam.bFindInitialOverlaps = false;
	queryParam.bReturnFaceIndex = true;
	FCollisionObjectQueryParams objectParam = objectParam.DefaultObjectQueryParam;

	auto pos = TileMesh->GetSocketLocation("Socket");
	auto rayStart = pos + GetActorForwardVector() * 2.0f;
	auto laserVector = GetActorForwardVector() * length;

	GetWorld()->LineTraceSingle(hit,rayStart, pos + laserVector, queryParam, objectParam);
	auto hitActor = hit.Actor.Get();
	return Cast<ABallPawn>(hitActor);
}

void AMagnetTile::PullBall(class ABallPawn* ball)
{
//	auto prim = Cast<UPrimitiveComponent>(ball->GetRootComponent());
//	FVector physicsLinearVelocity = prim->GetPhysicsLinearVelocity();
//	FVector actorForwardVectorNegative = -GetActorForwardVector();
//	float localDotProduct = FVector::DotProduct(physicsLinearVelocity, actorForwardVectorNegative);

	ball->AddVelocity(-GetActorForwardVector() * force, ball->GetActorLocation(), false);
}
