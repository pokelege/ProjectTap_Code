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

	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));


	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Game/Models/Ball"));

	ballMesh->SetWorldScale3D(FVector(0.25f));

	//ballMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	ballMesh->SetStaticMesh(tempMesh.Object);

	ballMesh->SetSimulatePhysics(false);

	ballMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ballMesh->AttachTo(ballCollision);
}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();
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

void ABallPawn::AddVelocity(const FVector& vel)
{
	ballCollision->AddImpulse(vel);
}
