// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "PlayerBall.h"


// Sets default values
APlayerBall::APlayerBall()
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
void APlayerBall::BeginPlay()
{
	Super::BeginPlay();
	FVector v(0.0f, 60000.0f, 0.0f);
	AddVelocity(v);
}

// Called every frame
void APlayerBall::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APlayerBall::AddVelocity(const FVector& vel)
{
	ballCollision->AddImpulse(vel);
}
