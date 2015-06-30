// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "SmokeCharacter.h"
#include "Controllers/SmokeAIController.h"

// Sets default values
ASmokeCharacter::ASmokeCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ASmokeAIController::StaticClass();
	GetCapsuleComponent()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetEnableGravity(false);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	smokeParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmokeParticle"));
	smokeParticleSystem->AttachTo(GetRootComponent());

	ConstructorHelpers::FObjectFinder<UParticleSystem> particleAsset(TEXT("/Game/Particles/P_Smoke_AI"));
	smokeParticleSystem->SetTemplate(particleAsset.Object);

	ConstructorHelpers::FObjectFinder<UBehaviorTree> behaviorAsset(TEXT("/Game/AI/SmokeBrain"));
	behavior = behaviorAsset.Object;
}

// Called when the game starts or when spawned
void ASmokeCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASmokeCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ASmokeCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

