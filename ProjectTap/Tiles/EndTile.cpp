// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameState.h"
#include "Engine/GameInstance.h"
#include "EndTile.h"

const FName AEndTile::END_MESH = FName("/Game/Models/EndTile");

AEndTile::AEndTile() : ATile()
{
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*END_MESH.ToString());
	TileMesh->SetStaticMesh(mesh.Object);

	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(40,40,80), false);
	}

	BoxCollision->bGenerateOverlapEvents = true;
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	auto spiralComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Spiral mesh" ) );
	ConstructorHelpers::FObjectFinder<UStaticMesh> spiralMesh(*FName("/Game/Models/SM_SpiralPlane").ToString());
	spiralComponent->SetStaticMesh(spiralMesh.Object);
	spiralComponent->AttachTo(BoxCollision);
	spiralComponent->SetRelativeLocation(FVector(0,0,85));

	auto particleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> particle(TEXT("/Game/Particles/P_Portal"));
	particleComponent->SetTemplate(particle.Object);
	particleComponent->AttachTo(BoxCollision);
	particleComponent->SetRelativeLocation(FVector(0,0,80));

	delegate.BindUFunction(this, TEXT("OnBeginTriggerOverlap"));
	BoxCollision->OnComponentBeginOverlap.Add(delegate);
}

void AEndTile::OnBeginTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	if (Cast<ABallPawn>(OtherActor) != nullptr)
	{
		auto pc = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		pc->SetSimulatePhysics(false);
		UGameInstance* gameInstance = GetGameInstance();
		FWorldContext* worldContext = gameInstance->GetWorldContext();
		UWorld* world = worldContext->World();
		AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
		if (gameState)
		{
			gameState->currentLevelToLoadWhenWin = loadLevelName;
			gameState->SetState(AProjectTapGameState::GAME_STATE_WIN);
		}

	}
}
