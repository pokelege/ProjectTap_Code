// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BaseRampTile.h"
#include "Pawns/BallPawn.h"

const FName ABaseRampTile::BASE_RAMP_CURVE_PATH = FName("/Game/Curves/Ramp");

ABaseRampTile::ABaseRampTile() : ATile()
{
	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(40,40,2), false);
		BoxCollision->bGenerateOverlapEvents = true;
	}

	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(*BASE_RAMP_CURVE_PATH.ToString());
	if(curve.Object != nullptr) rotationSequence = curve.Object;
	offset = CreateDefaultSubobject<USceneComponent>( TEXT( "Ramp offset" ) );
	offset->AttachTo(GetRootComponent());
	offset->SetRelativeLocation(FVector(40,0,0));
	TileMesh->AttachTo(offset,NAME_None,EAttachLocation::KeepWorldPosition);

	ConstructorHelpers::FObjectFinder<USoundWave> flipSoundFile( TEXT( "/Game/Sound/S_Click" ) );
	flipSound = CreateDefaultSubobject<UAudioComponent>( TEXT( "Flip Sound" ) );
	flipSound->SetSound( flipSoundFile.Object );
	flipSound->bAutoActivate = false;
	flipSound->AttachTo( BoxCollision );

	CancelHighlight();
	Disable();
}

void ABaseRampTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(activated)
	{
		float minTime, maxTime;
		rotationSequence->GetTimeRange(minTime, maxTime);
		if((time += DeltaTime) >= maxTime)
		{
			time = maxTime;
			deactivate();
		}
		float curveValue = rotationSequence->GetFloatValue(time);
		offset->SetRelativeRotation(FRotator(curveValue, 0, 0));
	}

}

void ABaseRampTile::activate()
{
	if(rotationSequence == nullptr || ball == nullptr || !IsEnabled() || activated) return;
	Super::activate();
	time = 0.0f;
	flipSound->Play();
}


void ABaseRampTile::HighlightEdge()
{
	if(material != nullptr)
	{
		material->SetScalarParameterValue("LerpEdgeColorHighlighted", 1);
		material->SetScalarParameterValue("LerpEdgePowerHighlighted", 1);
	}
}

void ABaseRampTile::CancelHighlightEdge()
{
	if(material != nullptr)
	{
		material->SetScalarParameterValue("LerpEdgeColorHighlighted", 0);
		material->SetScalarParameterValue("LerpEdgePowerHighlighted", 0);
	}
}

void ABaseRampTile::HighlightTile()
{
	if(material != nullptr)
	{
		material->SetScalarParameterValue("LerpBaseColorHighlighted", 1);
	}
}

void ABaseRampTile::CancelHighlightTile()
{
	if(material != nullptr)
	{
		material->SetScalarParameterValue("LerpBaseColorHighlighted", 0);
	}
}
