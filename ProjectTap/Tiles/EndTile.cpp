// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "EndTile.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameState.h"
#include "Engine/GameInstance.h"

const FName AEndTile::END_MESH = FName("/Game/Models/EndTile");

void AEndTile::StartTransporting()
{
	targetBall->setInvincibility(true);
	targetBallLastPosition = targetBall->GetActorLocation();
	currentAnimationTime = 0;
	ballToSocketCurveDuration = 0;
	transportScalingCurveDuration = 0;
	float minTime;
	if(ballToSocketCurve != nullptr)
	{
		ballToSocketCurve->GetTimeRange(minTime, ballToSocketCurveDuration);
	}
	if(transportScalingCurve != nullptr)
	{
		transportScalingCurve->GetTimeRange(minTime, transportScalingCurveDuration);
	}
	transporting = true;
}

void AEndTile::PlayTransport(const float &DeltaTime)
{
	currentAnimationTime += DeltaTime;
	FVector ballPosition = TileMesh->GetSocketLocation(FName("Transport"));
	FVector ballScale = transportScalingCurve == nullptr ? FVector(1,1,1) : transportScalingCurve->GetVectorValue(0);
	if(ballToSocketCurve != nullptr)
	{
		ballPosition = FMath::Lerp<FVector,FVector>(targetBallLastPosition, ballPosition, ballToSocketCurve->GetVectorValue(currentAnimationTime));
	}
	if(transportScalingCurve != nullptr)
	{
		ballScale = transportScalingCurve->GetVectorValue(currentAnimationTime);
	}

	if(currentAnimationTime > transportScalingCurveDuration && currentAnimationTime > ballToSocketCurveDuration)
	{
		UWorld* world = GetWorld();
		AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
		gameState->currentLevelToLoadWhenWin = loadLevelName;
		gameState->SetGameState( CustomGameState::GAME_STATE_WIN );
	}
	auto pc = Cast<UPrimitiveComponent>(targetBall->GetRootComponent());
	pc->SetWorldLocation(ballPosition);
	if ( targetBall->ballMesh )
	{
		targetBall->ballMesh->SetRelativeScale3D( ballScale );
	}
	else (pc->SetWorldScale3D(ballScale));
	pc->SetWorldRotation(FQuat(0,0,0,0));
}

AEndTile::AEndTile() : ATile()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*END_MESH.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	TileMesh->SetMobility( EComponentMobility::Static );
	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(40,40,80), false);
	}

	auto spiralComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Spiral mesh" ) );
	ConstructorHelpers::FObjectFinder<UStaticMesh> spiralMesh(*FName("/Game/Models/SM_SpiralPlane").ToString());
	spiralComponent->SetStaticMesh(spiralMesh.Object);
	spiralComponent->AttachTo(BoxCollision);
	spiralComponent->SetRelativeLocation(FVector(0,0,85));

	auto particleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> particle(TEXT("/Game/Particles/P_EndTile"));
	particleComponent->SetTemplate(particle.Object);
	particleComponent->AttachTo(BoxCollision);
	particleComponent->SetRelativeLocation(FVector(0,0,80));

	ConstructorHelpers::FObjectFinder<UCurveVector> ballToSocketCurveAsset(TEXT("/Game/Curves/BallToSocketCurve"));
	ConstructorHelpers::FObjectFinder<UCurveVector> transportScalingCurveAsset(TEXT("/Game/Curves/TransportScalingCurve"));
	ballToSocketCurve = ballToSocketCurveAsset.Object;
	transportScalingCurve = transportScalingCurveAsset.Object;
	delegate.BindUFunction(this, TEXT("OnBeginHit"));
	BoxCollision->OnComponentHit.Add(delegate);
}

void AEndTile::BeginDestroy()
{
	UWorld* world = GetWorld();
	AProjectTapGameState* gameState;
	if ( world != nullptr && ( gameState = world->GetGameState<AProjectTapGameState>()) != nullptr )
	{
		gameState->GameStateChanged.Remove( OnGameStateChangedDelegateHandle );
		OnGameStateChangedDelegateHandle.Reset();
	}
	Super::BeginDestroy();
}

void AEndTile::BeginPlay()
{
	Super::BeginPlay();
	UWorld* world = GetWorld();
	AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
	OnGameStateChangedDelegateHandle = gameState->GameStateChanged.AddUFunction( this , TEXT( "OnGameStateChanged" ) );
}

void AEndTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(transporting)
	{
		PlayTransport(DeltaTime);
	}
}

void AEndTile::OnGameStateChanged( const CustomGameState gameState )
{
	switch(gameState)
	{
		case CustomGameState::GAME_STATE_PLAYING:
			CanTransport = true;
			break;
		default:
			CanTransport = false;
	}
}

void AEndTile::OnBeginHit(class AActor* OtherActor,
						  class UPrimitiveComponent* OtherComp,
						  FVector NormalImpulse,
						  const FHitResult& Hit)
{
	if (Cast<ABallPawn>(OtherActor) != nullptr)
	{
		UWorld* world = GetWorld();
		AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
		if ( gameState && CanTransport )
		{
			gameState->SetGameState( CustomGameState::GAME_STATE_WINNING );
			targetBall = Cast<ABallPawn>(OtherActor);
			auto pc = Cast<UPrimitiveComponent>(targetBall->GetRootComponent());
			pc->SetSimulatePhysics(false);
			pc->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			StartTransporting();
		}
	}
}
