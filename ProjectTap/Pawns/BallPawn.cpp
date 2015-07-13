// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BallPawn.h"
#include "Engine/GameInstance.h"
#include "ProjectTapGameState.h"
#include "PawnCastingTrigger.h"
#include "BallPlayerStart.h"
#include "ConstrainingSpringArmComponent.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Runtime/UMG/Public/Blueprint/WidgetTree.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ABallPawn::ABallPawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ballCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	this->SetRootComponent(ballCollision);

	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));

	ballMesh->AttachTo(ballCollision);

	ballCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	ballCollision->SetSphereRadius(32.0f);

	ballCollision->SetSimulatePhysics(true);

	ballCollision->bGenerateOverlapEvents = true;

	ballCollision->SetCollisionProfileName(TEXT("Custom"));

	ballCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	ballCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	ballCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	ballCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	ballCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ballCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	ballCollision->GetBodyInstance()->bOverrideMass = true;
	ballCollision->GetBodyInstance()->MassInKg = 10.0f;

	//tileOverlapCollision->AttachTo(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Game/Models/Ball"));

	ballMesh->SetWorldScale3D(FVector(0.23f));

	//ballMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	ballMesh->SetStaticMesh(tempMesh.Object);
	ballMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	ballMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ballMesh->SetSimulatePhysics(false);

	spring = CreateDefaultSubobject<UConstrainingSpringArmComponent>(TEXT("Camera Spring"));
	spring->AttachTo(GetRootComponent());
	spring->bInheritPitch = false;
	spring->bInheritYaw = false;
	spring->bInheritRoll = false;
	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComponent->AttachTo(spring);

	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Game/Curves/BallDeath"));
	if(curve.Object != nullptr) dieSequence = curve.Object;

	ConstructorHelpers::FObjectFinder<USoundWave> dieSoundFile( TEXT( "/Game/Sound/S_Sizzle" ) );
	dieSound = CreateDefaultSubobject<UAudioComponent>( TEXT( "Die Sound" ) );
	dieSound->SetSound( dieSoundFile.Object );
	dieSound->bAutoActivate = false;
	dieSound->AttachTo( GetRootComponent() );

	ConstructorHelpers::FObjectFinder<UWidgetBlueprint> pauseMenuAssewt(TEXT("/Game/GUI/Pause"));
	pauseMenuInstance = Cast<UUserWidget>(pauseMenuAssewt.Object);

}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();

	ballCollision->AddImpulse(initialVelocity);
	trigger = GetWorld()->SpawnActor<APawnCastingTrigger>(GetActorLocation(), FRotator());
	trigger->SetBallPawn(this);
	bInvincible = false;
	spring->SetLockPosition(GetActorLocation());
	material = ballMesh->CreateDynamicMaterialInstance(0);
	material->SetVectorParameterValue(TEXT("Color"), FVector((float)FMath::Rand() / RAND_MAX, (float)FMath::Rand() / RAND_MAX, (float)FMath::Rand() / RAND_MAX));
	auto ctrl = GetWorld()->GetFirstPlayerController();
	if (ctrl != nullptr)
	{
		ctrl->InputComponent->BindAction("Pause", IE_Pressed, this, &ABallPawn::togglePauseMenu);
	}
}

// Called every frame
void ABallPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (Controller != nullptr)
	{
		Controller->InputComponent->BindAction("Pause", IE_Pressed, this, &ABallPawn::togglePauseMenu);
	}
	//udpate trigger position
	if (trigger != nullptr)
	{
		auto pos = GetActorLocation();
		pos.Z -= 40.0f;
		trigger->SetActorLocation(pos);
	}
	AProjectTapGameState* gameState = GetWorld()->GetGameState<AProjectTapGameState>();
	if(dying && gameState->GetState() == AProjectTapGameState::GAME_STATE_DYING)
	{
		currentDieTime += DeltaTime;
		if(dieSequence == nullptr)
		{
			material->SetScalarParameterValue(TEXT("DeathMask"), 1);
			gameState->SetState(AProjectTapGameState::GAME_STATE_GAME_OVER);
		}
		else
		{
			material->SetScalarParameterValue(TEXT("DeathMask"), dieSequence->GetFloatValue(currentDieTime));
			float min,max;
			dieSequence->GetValueRange(min,max);
			if(currentDieTime >= max)
			{
				gameState->SetState(AProjectTapGameState::GAME_STATE_GAME_OVER);
			}
		}
	}
}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

void ABallPawn::togglePauseMenu()
{
	/*auto state = Cast<AProjectTapGameState>(GetWorld()->GetGameState());
	auto ctrl = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (state->GetState() == AProjectTapGameState::GameState::GAME_STATE_PAUSE)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		pauseMenu->RemoveFromParent();
		ctrl->SetInputMode(FInputModeGameOnly::FInputModeGameOnly());
		state->SetState(AProjectTapGameState::GameState::GAME_STATE_PLAYING);

	}
	else if (state->GetState() != AProjectTapGameState::GameState::UNKNOWN)
	{
		pauseMenu = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), EBlueprintType::);
		auto inputMode = FInputModeUIOnly::FInputModeUIOnly();
		ctrl->SetInputMode(inputMode);
		pauseMenu->AddToViewport(1);		
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		state->SetState(AProjectTapGameState::GameState::GAME_STATE_PAUSE);
	}*/
}


void ABallPawn::AddVelocity(const FVector& vel, const FVector& resetPos, bool clearForce)
{
	ResetBallXYPosition(resetPos, vel);

	if (clearForce)
	{
		ballCollision->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
		ballCollision->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
	}

	ballCollision->AddImpulse(vel);

}


void ABallPawn::ResetBallXYPosition(const FVector& position, const FVector& newVel)
{
	FVector newPosition(position.X, position.Y, GetActorLocation().Z);

	auto vel = ballCollision->GetPhysicsLinearVelocity();
	auto clampedVel = newVel;
	clampedVel.Z = 0.0f;
	auto dp = FMath::Abs(FVector::DotProduct(vel.GetSafeNormal(), clampedVel.GetSafeNormal()));
	if (FMath::Acos(dp) * 180.0 / PI > 10.0f)
	{
		SetActorLocation(newPosition);
	}

}

void ABallPawn::ResetBallXYPosition(const FVector& position)
{
	FVector newPosition(position.X, position.Y, GetActorLocation().Z);

	SetActorLocation(newPosition);
}

void ABallPawn::Kill()
{
	AProjectTapGameState* gameState = GetWorld()->GetGameState<AProjectTapGameState>();
	if (gameState && !bInvincible && gameState->GetState() == AProjectTapGameState::GAME_STATE_PLAYING)
	{
		gameState->SetState(AProjectTapGameState::GAME_STATE_DYING);
		dieSound->Play();
		dying = true;
	}
}

void ABallPawn::FellOutOfWorld(const class UDamageType & dmgType)
{
	bInvincible = false;
	Kill();
}

void ABallPawn::setInvincibility(bool invincible)
{
	bInvincible = invincible;
}

void ABallPawn::setCamera(ABallPlayerStart* playerStart)
{
	if(playerStart != nullptr)
	{
		spring->lockX = playerStart->lockX;
		spring->lockY = playerStart->lockY;
		spring->lockZ = playerStart->lockZ;
		cameraComponent->SetWorldRotation( playerStart->camera->GetActorRotation() );
		cameraComponent->SetWorldLocation( playerStart->camera->GetActorLocation() );
		spring->SetTargetOffsetCustom( cameraComponent->RelativeLocation );
		cameraComponent->SetRelativeLocation( FVector( 0 , 0 , 0 ) );

		spring->bEnableCameraLag = playerStart->lagCamera;
		spring->CameraLagSpeed = playerStart->lagSpeed;
		spring->CameraLagMaxDistance = playerStart->lagMaxDistance;
	}
}

AActor *ABallPawn::getCamera()
{
	return cameraComponent == nullptr ? nullptr : cameraComponent->GetAttachmentRootActor();
}

bool ABallPawn::isDying()
{
	return dying;
}