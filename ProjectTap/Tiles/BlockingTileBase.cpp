// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BlockingTileBase.h"
#include "../Pawns/BallPawn.h"

// Sets default values
ABlockingTileBase::ABlockingTileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision->SetBoxExtent(FVector(1.0f, 1.0f, 1.0f));
	BoxCollision->bGenerateOverlapEvents = true;
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USoundWave> activateSoundFile( TEXT( "/Game/Sound/S_Swoosh" ) );
	activateSound = CreateDefaultSubobject<UAudioComponent>( TEXT( "Activate Sound" ) );
	activateSound->SetSound( activateSoundFile.Object );
	activateSound->bAutoActivate = false;
	activateSound->AttachTo( BoxCollision );
}

// Called when the game starts or when spawned
void ABlockingTileBase::BeginPlay()
{
	Super::BeginPlay();
	original = GetActorLocation();
}

// Called every frame
void ABlockingTileBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	auto pos = GetActorLocation();
	bool canRise = pos.Z - original.Z < move_distance_tolerance;
	bool canDesend = pos.Z - original.Z > FLT_EPSILON;

	if (activated && canRise)
	{
		pos.Z += move_speed * DeltaTime;
	}
	else if (activated && !canRise)
	{
		pos.Z = original.Z + move_distance_tolerance;
	}
	else if (!activated && canDesend)
	{
		pos.Z -= move_speed * DeltaTime;
		canDesend = pos.Z - original.Z > FLT_EPSILON;
		if (!canDesend)
		{
			Enable();
			pos.Z = original.Z;
		}
	}

	SetActorLocation(pos);

}

void ABlockingTileBase::lerpMaterialColorForCoolDown(const float& beta)
{
	if (material != nullptr)
	{
		material->SetScalarParameterValue(TEXT("LerpEdgeColorHighlighted"),1- beta);
		material->SetScalarParameterValue(TEXT("LerpEdgePowerHighlighted"),1- beta);
	}
}


void ABlockingTileBase::deactivate()
{
	Super::deactivate();
	time_counter = 0.0f;
	lerpMaterialColorForCoolDown(1.0f);
}
void ABlockingTileBase::activate()
{
	Super::activate();
	activateSound->Play();
}

bool ABlockingTileBase::CanKillBall()
{
	auto canKillBall = false;

	return canKillBall;
}

bool ABlockingTileBase::CanStopBall()
{
	return activated;
}
