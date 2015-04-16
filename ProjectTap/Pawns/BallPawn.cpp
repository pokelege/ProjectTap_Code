// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BallPawn.h"


// Sets default values
ABallPawn::ABallPawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ballCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));

	this->SetRootComponent(ballCollision);

	ballCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	ballCollision->SetSphereRadius(45.0f);

	ballCollision->SetSimulatePhysics(true);

	ballCollision->bGenerateOverlapEvents = true;

	ballCollision->SetCollisionProfileName(TEXT("Custom"));


	ballCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	ballCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	ballCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	ballCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ballCollision->GetBodyInstance()->bOverrideMass = true;
	ballCollision->GetBodyInstance()->MassInKg = 10.0f;

	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));


	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Game/Models/Ball"));

	ballMesh->SetWorldScale3D(FVector(0.25f));

	//ballMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	ballMesh->SetStaticMesh(tempMesh.Object);

	ballMesh->SetSimulatePhysics(false);

	ballMesh->AttachTo(ballCollision);

	
}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();

	auto temp = FVector(900.0f, 0.0f, 0.0f);
	ballCollision->AddImpulse(temp);
}

// Called every frame
void ABallPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
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
	}

	ballCollision->AddImpulse(vel);
}
