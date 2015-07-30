// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BaseRampTile.h"
#include "ProjectTapGameMode.h"
#include "Pawns/BallPawn.h"

const FName ABaseRampTile::BASE_RAMP_CURVE_PATH = FName("/Game/Curves/Ramp");
const GroundableInfo ABaseRampTile::groundableInfo = GroundableInfo(FVector(0,0,-2), false);
ABaseRampTile::ABaseRampTile() : ATile()
{
	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(40,40,2), false);
		BoxCollision->bGenerateOverlapEvents = true;
	}
	BoxCollision->SetCollisionResponseToAllChannels( ECollisionResponse::ECR_Ignore );
	BoxCollision->SetCollisionResponseToChannel( ECollisionChannel::ECC_Pawn , ECR_Overlap );
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECR_Overlap);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Overlap);
	BoxCollision->SetCollisionResponseToChannel( ECollisionChannel::ECC_Visibility , ECR_Block );
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

	ResetMeshOrientation();

	CancelHighlight();
	Disable();
}

const GroundableInfo* ABaseRampTile::GetGroundableInfo() const
{
	return &ABaseRampTile::groundableInfo;
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

void ABaseRampTile::BeginPlay()
{
	Super::BeginPlay();
	ResetMeshOrientation();
}

void ABaseRampTile::ResetMeshOrientation()
{
	UPrimitiveComponent* pc = Cast<UPrimitiveComponent>(RootComponent);

	switch (rotationDirection)
	{
	case Direction::XPlus:
	case Direction::Guess:
		pc->SetWorldRotation(FRotator(0, 0, 0));
		break;
	case Direction::xMinus:
		pc->SetWorldRotation(FRotator(0, 180, 0));
		break;
	case Direction::YPlus:
		pc->SetWorldRotation(FRotator(0, 90, 0));
		break;
	case Direction::yMinus:
		pc->SetWorldRotation(FRotator(0, 270, 0));
		break;
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
void ABaseRampTile::OnGameStateChanged( const CustomGameState newState )
{
	Super::OnGameStateChanged(newState);
	if ( newState == CustomGameState::GAME_STATE_PLAYING && lastPauseBall != nullptr )
	{
		Enable();
		HighlightTile();
		ball = lastPauseBall;
		lastPauseBall = nullptr;
	}
	else
	{
		CancelHighlight();
		Disable();
		lastPauseBall = ball;
		ball = nullptr;
	}
}

#if WITH_EDITOR
void ABaseRampTile::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		auto p = PropertyChangedEvent.Property;
		auto pName = p->GetName();

		//when currentEditorPathIndex property changes in editor
		//reset current moving tile's location to desinated node's location
		if (pName.Equals(TEXT("rotationDirection")))
		{
			ResetMeshOrientation();
		}

	}
}
#endif