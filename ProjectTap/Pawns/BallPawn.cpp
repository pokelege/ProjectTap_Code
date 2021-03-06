// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BallPawn.h"
#include "Engine/GameInstance.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Engine/Blueprint.h"
#include "ProjectTapGameState.h"
#include "BallPlayerStart.h"
#include "ConstrainingSpringArmComponent.h"
#include "CustomGameState.h"
#include "General/ProjectTapCameraComponent.h"
#include "General/ProjectTapCamera.h"
#include "Tiles/DeflectiveTile.h"
#include "Tiles/BlockingTileBase.h"

// Sets default values
ABallPawn::ABallPawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ballCollision = CreateDefaultSubobject<USphereComponent>( TEXT( "Collision" ) );
	this->SetRootComponent( ballCollision );

	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "BallMesh" ) );

	ballMesh->AttachTo( ballCollision );

	ballCollision->SetRelativeLocation( FVector( 0.0f , 0.0f , 0.0f ) );

	ballCollision->SetSphereRadius( 32.0f );

	ballCollision->SetSimulatePhysics( true );

	ballCollision->SetNotifyRigidBodyCollision( true );

	ballCollision->SetCollisionProfileName( TEXT( "Custom" ) );

	ballCollision->OnComponentHit.AddDynamic( this , &ABallPawn::OnHit );

	ballCollision->SetCollisionResponseToChannel( ECollisionChannel::ECC_Camera , ECollisionResponse::ECR_Ignore );
	ballCollision->SetCollisionResponseToChannel( ECollisionChannel::ECC_Visibility , ECollisionResponse::ECR_Ignore );
	ballCollision->SetCollisionResponseToChannel( ECollisionChannel::ECC_WorldStatic , ECollisionResponse::ECR_Block );
	ballCollision->SetCollisionResponseToChannel( ECollisionChannel::ECC_WorldDynamic , ECollisionResponse::ECR_Block );
	ballCollision->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	ballCollision->SetCollisionObjectType( ECollisionChannel::ECC_Pawn );
	ballCollision->GetBodyInstance()->bOverrideMass = true;
	ballCollision->GetBodyInstance()->MassInKg = 10.0f;
	ballCollision->BodyInstance.bUseCCD = true;

	//tileOverlapCollision->AttachTo(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh( TEXT( "/Game/Models/Ball" ) );

	//ballMesh->SetWorldScale3D(FVector(0.23f));

	//ballMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	ballMesh->SetStaticMesh( tempMesh.Object );
	ballMesh->SetCollisionResponseToChannel( ECollisionChannel::ECC_Visibility , ECollisionResponse::ECR_Ignore );
	ballMesh->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	ballMesh->SetSimulatePhysics( false );

	spring = CreateDefaultSubobject<UConstrainingSpringArmComponent>( TEXT( "Camera Spring" ) );
	spring->AttachTo( GetRootComponent() );
	spring->bInheritPitch = false;
	spring->bInheritYaw = false;
	spring->bInheritRoll = false;
	cameraComponent = CreateDefaultSubobject<UProjectTapCameraComponent>( TEXT( "Camera" ) );
	cameraComponent->AttachTo( spring );

	ConstructorHelpers::FObjectFinder<UCurveFloat> curve( TEXT( "/Game/Curves/BallDeath" ) );
	if ( curve.Object != nullptr ) dieSequence = curve.Object;

	ConstructorHelpers::FObjectFinder<UCurveFloat> curve2( TEXT( "/Game/Curves/BallHeart" ) );
	if ( curve.Object != nullptr ) heartBeatSequence = curve2.Object;

	ConstructorHelpers::FObjectFinder<USoundWave> dieSoundFile( TEXT( "/Game/Sound/S_Sizzle" ) );
	dieSound = CreateDefaultSubobject<UAudioComponent>( TEXT( "Die Sound" ) );
	dieSound->SetSound( dieSoundFile.Object );
	dieSound->bAutoActivate = false;
	dieSound->AttachTo( GetRootComponent() );

	ConstructorHelpers::FClassFinder<UUserWidget> pauseMenuClass( TEXT( "Class'/Game/GUI/Pause'" ) );
	pauseMenuBlueprint = pauseMenuClass.Class;

}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();

	ballCollision->AddImpulse( initialVelocity );

	bInvincible = false;
	spring->SetLockPosition( GetActorLocation() );
	material = ballMesh->CreateDynamicMaterialInstance( 0 );
	//material->SetVectorParameterValue(TEXT("Color"), FVector((float)FMath::Rand() / RAND_MAX, (float)FMath::Rand() / RAND_MAX, (float)FMath::Rand() / RAND_MAX));
	auto ctrl = GetWorld()->GetFirstPlayerController();
	if ( ctrl != nullptr )
	{
		ctrl->InputComponent->BindAction( "Pause" , IE_Pressed , this , &ABallPawn::togglePauseMenu );
		pauseMenuInstance = CreateWidget<UUserWidget>( ctrl , pauseMenuBlueprint );
	}

	SpawnCastingTrigger( BallCastType::CAST_BLOCKING );
	SpawnCastingTrigger( BallCastType::CAST_RAMP );
}

void ABallPawn::SpawnCastingTrigger( BallCastType type )
{
	if ( type == BallCastType::CAST_BLOCKING )
	{
		blockingTrigger = GetWorld()->SpawnActor<APawnCastingTrigger>();
		blockingTrigger->SetBallPawn( this );
		blockingTrigger->castType = type;
		blockingTrigger->RootComponent->SetWorldScale3D( FVector( 35.0f , 35.0f , 15.0f ) );
	}

	if ( type == BallCastType::CAST_RAMP )
	{
		rampTrigger = GetWorld()->SpawnActor<APawnCastingTrigger>();
		rampTrigger->SetBallPawn( this );
		rampTrigger->castType = type;
	}
}


// Called every frame
void ABallPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if ( Controller != nullptr )
	{
		Controller->InputComponent->BindAction( "Pause" , IE_Pressed , this , &ABallPawn::togglePauseMenu );
	}

	auto pos = GetActorLocation();
	pos.Z -= 40.0f;
	//udpate trigger position
	if ( blockingTrigger != nullptr )
	{
		blockingTrigger->SetActorLocation( pos );
	}
	if ( rampTrigger != nullptr )
	{
		rampTrigger->SetActorLocation( pos );
	}
	if ( heartBeatSequence != nullptr )
	{
		material->SetScalarParameterValue( TEXT( "HeartBeat" ) , heartBeatSequence->GetFloatValue( currentHeartTime ) );
		float min , max;
		heartBeatSequence->GetTimeRange( min , max );
		currentHeartTime += DeltaTime;
		if ( currentHeartTime >= max )
		{
			currentHeartTime = 0;
		}
	}


	AProjectTapGameState* gameState = GetWorld()->GetGameState<AProjectTapGameState>();
	if ( dying && gameState->GetState() == CustomGameState::GAME_STATE_DYING )
	{
		currentDieTime += DeltaTime;
		if ( dieSequence == nullptr )
		{
			material->SetScalarParameterValue( TEXT( "DeathMask" ) , 1 );
			gameState->SetGameState( CustomGameState::GAME_STATE_GAME_OVER );
		}
		else
		{
			material->SetScalarParameterValue( TEXT( "DeathMask" ) , dieSequence->GetFloatValue( currentDieTime ) );
			float min , max;
			dieSequence->GetTimeRange( min , max );
			if ( currentDieTime >= max )
			{
				gameState->SetGameState( CustomGameState::GAME_STATE_GAME_OVER );
			}
		}
	}

	UpdateResetTransition( DeltaTime );

	//update rigidbody wake up call
	if (rampTrigger->isOverlaping)
	{
		auto currentBallVelocity = ballCollision->GetPhysicsLinearVelocity();
		auto velMagnitude = currentBallVelocity.Size();
		if (velMagnitude < 60.0f)
		{
			auto clearXYVelocity = FVector(.0f, .0f, currentBallVelocity.Z);
			ballCollision->SetPhysicsLinearVelocity(clearXYVelocity);
		}

		ballCollision->WakeRigidBody();
	}

}

void ABallPawn::TransitionBallToProperLocation( const FVector& position , const FVector& newVelDir , float _transitionSpeed )
{
	lastAnchorPosition = FVector( position.X , position.Y , GetActorLocation().Z );

	auto initVec = GetActorLocation() - lastAnchorPosition;
	auto clearedZVelocity = FVector( newVelDir.X , newVelDir.Y , 0.0f );
	auto up = FVector::CrossProduct( clearedZVelocity , initVec );
	currentTransitionSpeed = _transitionSpeed;

	//when the ball is rolling towards the same direction
	//as the new velocity, only transition the ball
	//if the ball has not passed the tile
	auto notPassedTile = FMath::RadiansToDegrees( FMath::Acos( FVector::DotProduct( initVec , newVelDir ) ) ) < 10.0f;
	if ( up.IsZero() && notPassedTile )
	{
		transitionNormal = ADeflectiveTile::clampShortAxis( clearedZVelocity , true );
	}
	else
	{
		transitionNormal = FVector::CrossProduct( up , clearedZVelocity ).GetSafeNormal();
		transitionNormal = ADeflectiveTile::clampShortAxis( transitionNormal , true );
		bTransitioning = true;
	}

}


void ABallPawn::UpdateResetTransition( float dt )
{
	if ( bTransitioning )
	{
		auto vec = GetActorLocation() - lastAnchorPosition;
		auto moveDelta = -transitionNormal * currentTransitionSpeed * dt;


		auto dot = FVector::DotProduct( vec , transitionNormal );
		auto reachedPos = dot <= 0.0f;

		if ( reachedPos || bTransitionFinishNextFrame )
		{
			bTransitioning = false;
			currentTransitionSpeed = DEFUALT_TRANSITION_SPEED;
			SetActorLocation( GetActorLocation() + dot * -transitionNormal );
		}
		else
		{
			SetActorLocation( GetActorLocation() + moveDelta );
			dot = FVector::DotProduct( vec , transitionNormal );
			bTransitionFinishNextFrame = dot <= 0.0f;
		}
	}
}


// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent( class UInputComponent* InputComponent )
{
	Super::SetupPlayerInputComponent( InputComponent );
}

void ABallPawn::togglePauseMenu()
{
	auto state = Cast<AProjectTapGameState>( GetWorld()->GetGameState() );
	auto ctrl = Cast<APlayerController>( GetWorld()->GetFirstPlayerController() );

	if ( state->GetState() == CustomGameState::GAME_STATE_PAUSE )
	{
		UGameplayStatics::SetGamePaused( GetWorld() , false );
		pauseMenuInstance->RemoveFromParent();
		auto inputMode = FInputModeGameOnly::FInputModeGameOnly();
		ctrl->SetInputMode( inputMode );
		state->SetGameState( CustomGameState::GAME_STATE_PLAYING );

	}
	else if ( state->GetState() != CustomGameState::GAME_STATE_UNKNOWN
			  && state->GetMode() != CustomGameMode::GAME_MODE_MAIN_MENU )
	{
		auto inputMode = FInputModeUIOnly::FInputModeUIOnly();
		inputMode.SetWidgetToFocus( pauseMenuInstance->GetCachedWidget() );
		ctrl->SetInputMode( inputMode );
		pauseMenuInstance->AddToViewport( 1 );
		UGameplayStatics::SetGamePaused( GetWorld() , true );
		state->SetGameState( CustomGameState::GAME_STATE_PAUSE );
	}
}

void ABallPawn::AddVelocity( const FVector& vel , bool clearForce )
{
	if ( clearForce )
	{
		float velZ = ballCollision->GetPhysicsLinearVelocity().Z;
		ballCollision->SetPhysicsLinearVelocity( FVector( 0.0f , 0.0f , -9.8f ) );
	}
	ballCollision->SetPhysicsAngularVelocity( FVector( 0.0f , 0.0f , 0.0f ) );
	ballCollision->AddImpulse( vel );
}


void ABallPawn::AddVelocity( const FVector& vel , const FVector& resetPos , bool clearForce )
{
	TransitionBallToProperLocation( resetPos , vel );

	auto originalVelocity = ballCollision->GetPhysicsLinearVelocity();
	originalVelocity.Z = .0f;

	auto noZNewVelocity = vel;
	noZNewVelocity.Z = .0f;

	auto dot = FVector::DotProduct(originalVelocity, noZNewVelocity);
	auto angle = FMath::RadiansToDegrees(FMath::Acos(dot));

	auto newVel = vel;
	if (angle <= 10.0f)
	{
		newVel += newVel * .5f;
	}

	if (clearForce)
	{
		ballCollision->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
		ballCollision->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
	}

	newVel.Z = vel.Z;
	ballCollision->AddImpulse(newVel);
}


void ABallPawn::ResetBallXYPosition( const FVector& position )
{
	FVector newPosition( position.X , position.Y , GetActorLocation().Z );

	SetActorLocation( newPosition );
}

void ABallPawn::Kill()
{
	AProjectTapGameState* gameState = GetWorld()->GetGameState<AProjectTapGameState>();
	if ( gameState && !bInvincible && gameState->GetState() == CustomGameState::GAME_STATE_PLAYING && gameState->GetMode() != CustomGameMode::GAME_MODE_MAIN_MENU )
	{
		gameState->SetGameState( CustomGameState::GAME_STATE_DYING );
		dieSound->Play();
		dying = true;
		spring->lockX = true;
		spring->lockY = true;
		spring->lockZ = true;
		spring->SetLockPosition( GetActorLocation() );
		//spring->SetTargetOffsetCustom( cameraComponent->RelativeLocation );
	}
}

void ABallPawn::FellOutOfWorld( const class UDamageType & dmgType )
{
	bInvincible = false;
	Kill();
}

void ABallPawn::setInvincibility( bool invincible )
{
	bInvincible = invincible;
}

void ABallPawn::setCamera( ABallPlayerStart* playerStart )
{
	if ( playerStart != nullptr )
	{
		spring->lockX = playerStart->lockX;
		spring->lockY = playerStart->lockY;
		spring->lockZ = playerStart->lockZ;
		cameraComponent->SetWorldRotation( playerStart->camera->GetActorRotation() );
		cameraComponent->SetWorldLocation( playerStart->camera->GetActorLocation() );
		cameraComponent->PostProcessSettings = Cast<UProjectTapCameraComponent>( playerStart->camera->GetComponentByClass( UProjectTapCameraComponent::StaticClass() ) )->PostProcessSettings;
		spring->SetTargetOffsetCustom( cameraComponent->RelativeLocation );
		cameraComponent->SetRelativeLocation( FVector( 0 , 0 , 0 ) );

		spring->bEnableCameraLag = playerStart->lagCamera;
		spring->CameraLagSpeed = playerStart->lagSpeed;
		spring->CameraLagMaxDistance = playerStart->lagMaxDistance;
	}
}

void ABallPawn::OnHit( class AActor* OtherActor ,
class UPrimitiveComponent* OtherComp ,
	FVector NormalImpulse ,
	const FHitResult& Hit )

{
	if ( OtherActor->IsA( ABlockingTileBase::StaticClass() ) )
	{
		auto tile = Cast<ABlockingTileBase>( OtherActor );
		if ( tile->isActivated() && tile->IsEnabled() )
		{
			AddVelocity( FVector::ZeroVector , true );
		}
	}
}

UProjectTapCameraComponent* ABallPawn::GetCamera()
{
	return cameraComponent;
}

bool ABallPawn::isDying()
{
	return dying;
}

