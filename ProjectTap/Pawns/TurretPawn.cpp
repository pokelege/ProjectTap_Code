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

	float dot = FVector::DotProduct(forward, turretToBallNormal);
	if(dot > FMath::DegreesToRadians(FOV)) return;
	FHitResult hit;
	FCollisionQueryParams p;
	FCollisionResponseParams p2;
	if(world->LineTraceSingle(hit,translate, player->GetTransform().GetTranslation(),ECollisionChannel::ECC_MAX,p,p2) && hit.GetActor() == player)
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

