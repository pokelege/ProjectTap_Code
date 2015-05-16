// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameState.h"
#include "Engine/GameInstance.h"
#include "BallPawn.h"
#include "../Tiles/Tile.h"
#include "PawnCastingTrigger.h"


// Sets default values
ABallPawn::ABallPawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ballCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	this->SetRootComponent(ballCollision);

	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));

	ballMesh->AttachTo(ballCollision);

	ballCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	ballCollision->SetSphereRadius(32.0f);

	ballCollision->SetSimulatePhysics(true);

	ballCollision->bGenerateOverlapEvents = true;

	ballCollision->SetCollisionProfileName(TEXT("Custom"));

	ballCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	ballCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	ballCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	ballCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	ballCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ballCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	ballCollision->GetBodyInstance()->bOverrideMass = true;
	ballCollision->GetBodyInstance()->MassInKg = 10.0f;	

	//tileOverlapCollision->AttachTo(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Game/Models/Ball"));

	ballMesh->SetWorldScale3D(FVector(0.23f));

	//ballMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	ballMesh->SetStaticMesh(tempMesh.Object);
	ballMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	ballMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ballMesh->SetSimulatePhysics(false);


}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();

	ballCollision->AddImpulse(initialVelocity);
	trigger = GetWorld()->SpawnActor<APawnCastingTrigger>(GetActorLocation(), FRotator());
	trigger->SetBallPawn(this);
	bInvincible = false;
}

// Called every frame
void ABallPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//udpate trigger position
	if (trigger != nullptr)
	{
		auto pos = GetActorLocation();
		pos.Z -= 40.0f;
		trigger->SetActorLocation(pos);
	}
}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

void ABallPawn::AddVelocity(const FVector& vel, const FVector& resetPos, bool clearForce)
{
	ResetBallXYPosition(resetPos, vel);

	if (clearForce)
	{
		ballCollision->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
		ballCollision->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
	}

	ballCollision->AddImpulse(vel);

}


void ABallPawn::ResetBallXYPosition(const FVector& position, const FVector& newVel)
{
	FVector newPosition(position.X, position.Y, GetActorLocation().Z);
	
	auto vel = ballCollision->GetPhysicsLinearVelocity();
	auto dp = FMath::Abs(FVector::DotProduct(vel, newVel.GetSafeNormal()));
	if (FMath::Acos(dp) * 180.0 / PI > 10.0f)
	{
		SetActorLocation(newPosition);
	}

}

void ABallPawn::ResetBallXYPosition(const FVector& position)
{
	FVector newPosition(position.X, position.Y, GetActorLocation().Z);

	SetActorLocation(newPosition);

}

void ABallPawn::Kill()
{
	if (!bInvincible)
	{
		AProjectTapGameState* gameState = GetWorld()->GetGameState<AProjectTapGameState>();
		if (gameState) gameState->SetState(AProjectTapGameState::GAME_STATE_GAME_OVER);
		if (trigger != nullptr) trigger->Destroy();
		Destroy();
	}
}

void ABallPawn::FellOutOfWorld(const class UDamageType & dmgType)
{
	bInvincible = false;
	Kill();
}

void ABallPawn::setInvincibility(bool invincible)
{
	bInvincible = invincible;
}