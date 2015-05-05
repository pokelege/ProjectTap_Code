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

	ballMesh->SetWorldScale3D(FVector(0.25f));

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

void ABallPawn::AddVelocity(const FVector& vel, bool clearForce)
{
	if (clearForce)
	{
		ballCollision->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
		ballCollision->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
	}

	ballCollision->AddImpulse(vel);
}


void ABallPawn::ResetBallXYPosition(const FVector& position)
{
	FVector newPosition(position.X, position.Y, GetActorLocation().Z);

	auto newDir = newPosition - GetActorLocation();
	if (FVector::DotProduct(newDir, ballCollision->GetPhysicsLinearVelocity()) > 0.0f)
	{
		SetActorLocation(newPosition);
	}

}


void ABallPawn::Kill()
{
	UGameInstance* gameInstance = GetGameInstance();
	FWorldContext* worldContext = gameInstance->GetWorldContext();
	UWorld* world = worldContext->World();
	AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
	if(gameState) gameState->SetState(AProjectTapGameState::GAME_STATE_GAME_OVER);
	if (trigger != nullptr) trigger->Destroy();
	Destroy();
}

void ABallPawn::FellOutOfWorld(const class UDamageType & dmgType)
{
	Kill();
}

