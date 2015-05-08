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
	forceMultiplier = 2000.0f;

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
			ball->AddVelocity((forceMultiplier * FVector(1,0,0)) + FVector(0,0,additionalZForce), GetActorLocation());
			break;
		case Direction::xMinus:
			ball->AddVelocity((forceMultiplier * FVector(-1, 0, 0)) + FVector(0, 0, additionalZForce), GetActorLocation());
			break;
		case Direction::YPlus:
			ball->AddVelocity((forceMultiplier * FVector(0, 1, 0)) + FVector(0, 0, additionalZForce), GetActorLocation());
			break;
		case Direction::yMinus:
			ball->AddVelocity((forceMultiplier * FVector(0, -1, 0)) + FVector(0, 0, additionalZForce), GetActorLocation());
			break;
	}


}

