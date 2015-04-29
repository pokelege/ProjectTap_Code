// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameState.h"
#include "Engine/GameInstance.h"
#include "BallPawn.h"
#include "../Tiles/Tile.h"
#include "../EmptyComponent.h"


// Sets default values
ABallPawn::ABallPawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<UEmptyComponent>(TEXT("Empty root"));
	this->SetRootComponent(root);

	ballCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	tileOverlapCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TileOverlap"));
	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));

	ballCollision->AttachTo(root);
	tileOverlapCollision->AttachTo(root);
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
	tileOverlapCollision->bGenerateOverlapEvents = true;

	//tileOverlapCollision->AttachTo(RootComponent);
	tileOverlapCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	tileOverlapCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	tileOverlapCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	tileOverlapCollision->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
	tileOverlapCollision->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	FScriptDelegate beginOverLap;
	beginOverLap.BindUFunction(this, "OnBeginTriggerOverlap");
	tileOverlapCollision->OnComponentBeginOverlap.Add(beginOverLap);

	FScriptDelegate beginEndLap;
	beginEndLap.BindUFunction(this, "OnEndTriggerOverlap");
	tileOverlapCollision->OnComponentEndOverlap.Add(beginEndLap);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Game/Models/Ball"));

	ballMesh->SetWorldScale3D(FVector(0.25f));

	//ballMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	ballMesh->SetStaticMesh(tempMesh.Object);
	ballMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	ballMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ballMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();

	ballCollision->AddImpulse(initialVelocity);
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

void ABallPawn::AddVelocity(const FVector& vel, bool clearForce)
{
	
	if (clearForce)
	{
		ballCollision->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
		ballCollision->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
	}

	ballCollision->AddImpulse(vel);
}


void ABallPawn::ResetBallXYPosition(const FVector& position)
{
	FVector newPosition(position.X, position.Y, GetActorLocation().Z);
	SetActorLocation(newPosition);
}


void ABallPawn::Kill()
{
	UGameInstance* gameInstance = GetGameInstance();
	FWorldContext* worldContext = gameInstance->GetWorldContext();
	UWorld* world = worldContext->World();
	AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
	if(gameState) gameState->SetState(AProjectTapGameState::GAME_STATE_GAME_OVER);
	Destroy();
}

void ABallPawn::FellOutOfWorld(const class UDamageType & dmgType)
{
	Kill();
}

void ABallPawn::OnBeginTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	auto tile = Cast<ATile>(OtherActor);
	if (tile != nullptr)
	{
		tile->deactivate();
		tile->Disable();
		tile->turnOffHighlight();
	}
}

void ABallPawn::OnEndTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	auto tile = Cast<ATile>(OtherActor);
	if (tile != nullptr)
	{
		tile->Enable();
		tile->CancelHighlight();
	}
}