// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "JumpTile.h"

const FName AJumpTile::JUMP_MESH_PATH = FName("/Game/Models/Jump");

AJumpTile::AJumpTile() : ABaseRampTile()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*JUMP_MESH_PATH.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	TileMesh->SetRelativeLocation(FVector(1,0,0), false, nullptr);
}

void AJumpTile::BeginPlay()
{
	Super::BeginPlay();
}

void AJumpTile::activate()
{
	if(rotationSequence == nullptr || target == nullptr || ball == nullptr || activated) return;
	Super::activate();
	//todo check if mass is the same
	calculateImpulse();
	ball->ballCollision->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
	ball->ballCollision->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
	ball->ballCollision->AddImpulse(force);
}

void AJumpTile::calculateImpulse()
{
	float vz = FMath::Sqrt(2 * -GetWorld()->GetGravityZ() * height);
	float forceZ = ball->ballCollision->GetMass() * vz / duration;
	
	auto dir = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	auto distance = FVector::Dist(target->GetActorLocation(), GetActorLocation());
	auto horizontalAcceleration = 2 * distance / FMath::Square(2 * duration);
	auto forceX = ball->ballCollision->GetMass() * horizontalAcceleration;
	force = dir * forceX * 2 * duration + FVector(0.0f, 0.0f, forceZ) * duration;
}