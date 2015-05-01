// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Ramp.h"

const FName ARamp::RAMP_MESH_PATH = FName("/Game/Models/Ramp");

ARamp::ARamp(): ABaseRampTile()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*RAMP_MESH_PATH.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	TileMesh->SetRelativeLocation(FVector(1,0,0), false, nullptr);

	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(1,1,1), false);
		BoxCollision->SetRelativeLocation(FVector(0, 0, -10), false, nullptr);
		BoxCollision->AddLocalOffset(FVector(0, 0, -10));
		BoxCollision->bGenerateOverlapEvents = true;
	}

	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(*RAMP_CURVE_PATH.ToString());
	if(curve.Object != nullptr) rotationSequence = curve.Object;

	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(40.0f, 40.0f, 2.0f));

	forceMultiplier = 2000.0f;


	baseColorHighlighted = FLinearColor(0.0f, 5.0f, .0f);
	glowColorHighlighted = FLinearColor(2.0f, 1.7f, .0f);
	baseColor = FLinearColor(1.0f, 1.0f, 1.0f);
	glowColor = FLinearColor(1.0f, .7f, .0f);
	glowPowerHighlighted = 100.0f;
	CancelHighlight();
	Disable();
}

void ARamp::BeginPlay()
{
	Super::BeginPlay();
	UPrimitiveComponent* pc = Cast<UPrimitiveComponent>(RootComponent);
	switch(rotationDirection)
	{
		case Direction::XPlus:
			pc->SetWorldRotation(FRotator(0,0,0));
			break;
		case Direction::xMinus:
			pc->SetWorldRotation(FRotator(0,180,0));
			break;
		case Direction::YPlus:
			pc->SetWorldRotation(FRotator(0,90,0));
			break;
		case Direction::yMinus:
			pc->SetWorldRotation(FRotator(0,270,0));
			break;
	}
}

void ARamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARamp::activate()
{
	if(rotationSequence == nullptr || ball == nullptr || activated || !IsEnabled()) return;
	Super::activate();
	
	switch(rotationDirection)
	{
		case Direction::XPlus:
			ball->AddVelocity((forceMultiplier * FVector(1,0,0)) + FVector(0,0,additionalZForce));
			break;
		case Direction::xMinus:
			ball->AddVelocity((forceMultiplier * FVector(-1,0,0)) + FVector(0,0,additionalZForce));
			break;
		case Direction::YPlus:
			ball->AddVelocity((forceMultiplier * FVector(0,1,0)) + FVector(0,0,additionalZForce));
			break;
		case Direction::yMinus:
			ball->AddVelocity((forceMultiplier * FVector(0,-1,0)) + FVector(0,0,additionalZForce));
			break;
	}
}



void ARamp::Highlight(bool litTile, bool litEdge) 
{
	Super::Highlight(false, true);
}

