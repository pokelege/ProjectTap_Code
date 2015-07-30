// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BlockingTileBase.h"
#include "../Pawns/BallPawn.h"

// Sets default values
ABlockingTileBase::ABlockingTileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision->SetBoxExtent(FVector(40.0f, 40.0f, 80.0f));
	BoxCollision->bGenerateOverlapEvents = true;
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USoundBase> activateSoundFile( TEXT( "/Game/Sound/S_GearClicking" ) );
	ActivateSound = activateSoundFile.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> deactivateSoundFile( TEXT( "/Game/Sound/S_GearClickingReverse" ) );
	DeactivateSound = deactivateSoundFile.Object;
	SoundPlayer = CreateDefaultSubobject<UAudioComponent>( TEXT( "Activate Sound" ) );
	SoundPlayer->bAutoActivate = false;
	SoundPlayer->AttachTo( BoxCollision );
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
		if ( SoundPlayer->IsPlaying() ) SoundPlayer->Stop();
	}
	else if (!activated && canDesend)
	{
		pos.Z -= move_speed * DeltaTime;
		canDesend = pos.Z - original.Z > FLT_EPSILON;
		if (!canDesend)
		{
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
	SoundPlayer->SetSound( DeactivateSound );
	SoundPlayer->Play();
}
void ABlockingTileBase::activate()
{
	Super::activate();
	SoundPlayer->SetSound( ActivateSound );
	SoundPlayer->Play();
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
