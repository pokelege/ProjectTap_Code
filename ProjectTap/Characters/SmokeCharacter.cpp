// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "SmokeCharacter.h"
#include "Controllers/SmokeAIController.h"

// Sets default values
ASmokeCharacter::ASmokeCharacter()
{
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