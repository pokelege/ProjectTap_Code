// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "JumpTile.h"

const FName AJumpTile::JUMP_MESH_PATH = FName("/Game/Models/Ramp");

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
	calculatePhysics();
	ball->ballCollision->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
	ball->ballCollision->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
	ball->ballCollision->AddForce(force);
}

void AJumpTile::calculatePhysics()
{
	float Viy = (height - (0.5f * ball->GetWorld()->GetGravityZ() * (duration/2) * (duration / 2))) / (duration / 2);
	float Ay = Viy/ (duration / 2);
	float Fy = Ay * ball->ballCollision->GetBodyInstance()->MassInKg;
	float Fgrav = ball->GetWorld()->GetGravityZ() * ball->ballCollision->GetBodyInstance()->MassInKg;

	FVector xVector = target->GetTransform().GetLocation() - GetTransform().GetLocation();
	float Vix = (xVector.Size() * 2) /duration;
	float Ax = -Vix / duration;
	float Fx = Ax * ball->ballCollision->GetBodyInstance()->MassInKg;
	force = (Fx * xVector.GetSafeNormal()) + FVector(0,0,Fy - Fgrav);
}