// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BaseRampTile.h"

const FName ABaseRampTile::BASE_RAMP_CURVE_PATH = FName("/Game/Curves/Ramp");

ABaseRampTile::ABaseRampTile() : ATile()
{
	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(40,40,2), false);
		BoxCollision->SetRelativeLocation(FVector(0, 0, -10), false, nullptr);
		BoxCollision->AddLocalOffset(FVector(0, 0, -10));
		BoxCollision->bGenerateOverlapEvents = true;
	}

	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(*BASE_RAMP_CURVE_PATH.ToString());
	if(curve.Object != nullptr) rotationSequence = curve.Object;
	offset = CreateDefaultSubobject<USceneComponent>( TEXT( "Ramp offset" ) );
	offset->AttachTo(GetRootComponent());
	offset->SetRelativeLocation(FVector(40,0,0));
	TileMesh->AttachTo(offset);
	baseColorHighlighted = FLinearColor(0.0f, 5.0f, .0f);
	glowColorHighlighted = FLinearColor(2.0f, 1.7f, .0f);
	baseColor = FLinearColor(1.0f, 1.0f, 1.0f);
	glowColor = FLinearColor(1.0f, 1.0f, 1.0f);
	glowPowerHighlighted = 100.0f;
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

}

void ABaseRampTile::Highlight(bool litTile, bool litEdge)
{
	Super::Highlight(false, true);
}

