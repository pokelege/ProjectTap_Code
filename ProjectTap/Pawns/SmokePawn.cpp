// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "SmokePawn.h"
#include "Controllers/SmokeAIController.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameState.h"
// Sets default values
ASmokePawn::ASmokePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ASmokeAIController::StaticClass();
	collision = CreateDefaultSubobject<USphereComponent>(TEXT("SmokeCollision"));
	SetRootComponent(collision);
	collision->SetSimulatePhysics(true);
	collision->SetEnableGravity(false);
	collision->SetCollisionProfileName(FName("Pawn"));
	collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	smokeParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmokeParticle"));
	smokeParticleSystem->AttachTo(GetRootComponent());

	ConstructorHelpers::FObjectFinder<UParticleSystem> particleAsset(TEXT("/Game/Particles/P_Smoke_AI"));
	smokeParticleSystem->SetTemplate(particleAsset.Object);

	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("SmokeMovement"));
	movement->SetUpdatedComponent(GetRootComponent());
	movement->MaxSpeed = 100.0f;
	movement->Acceleration = 100.0f;
	movement->Deceleration = 0.0;
	ConstructorHelpers::FObjectFinder<UBehaviorTree> behaviorAsset(TEXT("/Game/AI/SmokeBrain"));
	behavior = behaviorAsset.Object;
}

// Called when the game starts or when spawned
void ASmokePawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASmokePawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	auto ai = Cast<AAIController>( GetController() );
	if ( ai != nullptr )
	{
		FAIMoveRequest aim;
		AProjectTapGameState* gamestate = Cast<AProjectTapGameState>( GetWorld()->GetGameState() );
		aim.SetGoalActor(gamestate->CurrentPawn);
		ai->MoveTo(aim);
	}
}

// Called to bind functionality to input
void ASmokePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

UPawnMovementComponent *ASmokePawn::GetMovementComponent() const
{
	return movement;
}
