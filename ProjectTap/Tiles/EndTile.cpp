// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "EndTile.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameState.h"
#include "Engine/GameInstance.h"

const FName AEndTile::END_MESH = FName("/Game/Models/EndTile");

AEndTile::AEndTile() : ATile()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh( *END_MESH.ToString() );
	TileMesh->SetStaticMesh( mesh.Object );
	TileMesh->SetMobility( EComponentMobility::Static );
	if ( BoxCollision )
	{
		BoxCollision->SetBoxExtent( FVector( 40 , 40 , 80 ) , false );
	}

	auto particleComponent = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "Particle" ) );
	ConstructorHelpers::FObjectFinder<UParticleSystem> particle( TEXT( "/Game/Particles/P_EndTile" ) );
	particleComponent->SetTemplate( particle.Object );
	particleComponent->AttachTo( BoxCollision );
	particleComponent->SetRelativeLocation( FVector( 0 , 0 , 80 ) );

	ConstructorHelpers::FObjectFinder<UCurveVector> ballToSocketCurveAsset( TEXT( "/Game/Curves/BallToSocketCurve" ) );
	ConstructorHelpers::FObjectFinder<UCurveVector> transportScalingCurveAsset( TEXT( "/Game/Curves/TransportScalingCurve" ) );
	soundToPlayCurve = ConstructorHelpers::FObjectFinder<UCurveFloat>( TEXT( "/Game/Curves/SoundToPlayCurve" ) ).Object;
	ballToSocketCurve = ballToSocketCurveAsset.Object;
	transportScalingCurve = transportScalingCurveAsset.Object;
	delegate.BindUFunction( this , TEXT( "OnBeginHit" ) );
	BoxCollision->OnComponentHit.Add( delegate );

	ConstructorHelpers::FObjectFinder<USoundBase> sound1( TEXT( "/Game/Sound/S_Warp1" ) );
	ConstructorHelpers::FObjectFinder<USoundBase> sound2( TEXT( "/Game/Sound/S_Warp2" ) );
	sounds.Add( sound1.Object );
	sounds.Add( sound2.Object );
	audioPlayer = CreateDefaultSubobject<UAudioComponent>( TEXT( "Audio Player" ) );
	audioPlayer->bAutoActivate = false;
	audioPlayer->AttachTo( particleComponent );
	BoxCollision->bGenerateOverlapEvents = true;
}

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
	if ( soundToPlayCurve == nullptr ) return;
	int soundIndex = soundToPlayCurve->GetFloatValue( currentAnimationTime );
	if ( soundIndex >= 0 && soundIndex < sounds.Num() )
	{
		if ( audioPlayer->Sound != sounds[soundIndex] )
		{
			audioPlayer->Stop();
			audioPlayer->SetSound( sounds[soundIndex] );
			audioPlayer->Play();
		}
	}
}

void AEndTile::BeginDestroy()
{
	Super::BeginDestroy();
}

void AEndTile::BeginPlay()
{
	Super::BeginPlay();
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
	Super::OnGameStateChanged(gameState);
	switch(gameState)
	{
		case CustomGameState::GAME_STATE_PLAYING:
			CanTransport = true;
			break;
		default:
			CanTransport = false;
	}
}

void AEndTile::EndLevel(ABallPawn* ball)
{
	UWorld* world = GetWorld();
	AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
	if ( gameState && CanTransport )
	{
		gameState->SetGameState( CustomGameState::GAME_STATE_WINNING );
		targetBall = ball;
		auto pc = Cast<UPrimitiveComponent>(targetBall->GetRootComponent());
		pc->SetSimulatePhysics(false);
		pc->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		StartTransporting();
	}
}
