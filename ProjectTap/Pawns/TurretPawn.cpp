// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Engine/GameInstance.h"
#include "BallPawn.h"
#include "ProjectTapGameState.h"
#include "TurretPawn.h"

const FName ATurretPawn::MESH = FName("/Game/Models/Turret");

// Sets default values
ATurretPawn::ATurretPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*MESH.ToString());
	UStaticMeshComponent* TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Turret mesh" ) );
	TurretMesh->SetStaticMesh(mesh.Object);
	this->SetRootComponent(TurretMesh);
	//nozzle->SetWorldTransform();
}

// Called when the game starts or when spawned
void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATurretPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	FVector translate, forward;
	if(nozzle == nullptr)
	{
		translate = this->GetTransform().GetTranslation();
		forward = this->GetActorForwardVector();
	}
	else
	{
		translate = nozzle->GetComponentTransform().GetTranslation();
		forward = nozzle->GetForwardVector();
	}

	UGameInstance* gameInstance = GetGameInstance();
	FWorldContext* worldContext = gameInstance->GetWorldContext();
	UWorld* world = worldContext->World();
	AProjectTapGameState* gameState = world->GetGameState<AProjectTapGameState>();
	ABallPawn* player = gameState->CurrentPawn;
	if(player == nullptr) return;
	FVector turretToBallNormal = (player->GetTransform().GetTranslation() - translate);
	turretToBallNormal.Normalize();

	float dot = FVector::DotProduct(turretToBallNormal,forward);
	float radians = FMath::Cos(FMath::DegreesToRadians(FOV));
	if(dot < radians) return;
	FHitResult hit(ForceInit);
	FCollisionQueryParams p = FCollisionQueryParams(FName(TEXT("Tracing")), true, this);
	p.bTraceComplex = true;
	p.bTraceAsyncScene = true;
	p.bReturnPhysicalMaterial = false;

	FCollisionResponseParams p2;
	world->LineTraceSingle(hit,translate, maxDistance * forward,ECollisionChannel::ECC_Pawn,p);
	//todo if actually found player
	if(hit.GetActor() == player)
 	{
		//todo shoot
		player->Kill();
 	}
}

// Called to bind functionality to input
void ATurretPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

