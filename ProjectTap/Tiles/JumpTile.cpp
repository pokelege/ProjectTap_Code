// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "JumpTile.h"
#include "../Pawns/BallPawn.h"

const FName AJumpTile::JUMP_MESH_PATH = FName("/Game/Models/Jump");

AJumpTile::AJumpTile() : ABaseRampTile()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*JUMP_MESH_PATH.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
}

void AJumpTile::BeginPlay()
{
	Super::BeginPlay();
	if(target == nullptr) return;
	auto dir = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
	UPrimitiveComponent* pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldRotation(dir.Rotation());
}

void AJumpTile::SetWaitForBall()
{
	isBallComing = true;
}

void AJumpTile::activate()
{
	if(rotationSequence == nullptr || target == nullptr || ball == nullptr || !IsEnabled() || activated) return;
	Super::activate();
	auto dir = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	ball->TransitionBallToProperLocation(GetActorLocation(), dir);
	target->SetWaitForBall();

	calculateImpulse();
	ball->ballCollision->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
	ball->ballCollision->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
	ball->ballCollision->AddImpulse(impulse);
}

void AJumpTile::calculateImpulse()
{
	//h = 1/2*a*t^2
	//t = sqrt(2h/a)
	float t = FMath::Sqrt(2 * height  / -GetWorld()->GetGravityZ()); 
	//vf = vi + at
	//vf == 0
	//vi = -at
	float velocityZ = -GetWorld()->GetGravityZ() * t;
	auto dir = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	auto startPos = ball->GetActorLocation();
	auto distance = FVector::Dist(target->GetActorLocation(), startPos);

	//d = 1/2 * a *t^2
	//a = 2d / t^2
	auto horizontaAcceleration = 2 * distance / FMath::Square(2 * t);//t *2 because ball goes up and down
	auto mass = ball->ballCollision->GetMass();
	auto forceX = mass * horizontaAcceleration;
	auto forceZ = mass * velocityZ;

	//impuls(change in momentum) = f * t
	impulse = (dir * forceX + FVector(0.0f, 0.0f, forceZ)) * t;
}

void AJumpTile::HighlightEdge()
{
	Super::HighlightEdge();
	if ( target != nullptr )
	{
		target->Super::HighlightEdge();
	}
}

void AJumpTile::CancelHighlightEdge()
{
	Super::CancelHighlightEdge();
	if ( target != nullptr )
	{
		target->Super::CancelHighlightEdge();
	}
}

void AJumpTile::HighlightTile()
{
	Super::HighlightTile();
	if ( target != nullptr )
	{
		target->Super::HighlightTile();
		if (isBallComing)
		{
			isBallComing = false;
			auto newDir = (GetActorLocation() - target->GetActorLocation()).GetSafeNormal();
			ball->AddVelocity(newDir * 2000.0f, false);
		}
	}
}

void AJumpTile::CancelHighlightTile()
{
	Super::CancelHighlightTile();
	if ( target != nullptr )
	{
		target->Super::CancelHighlightTile();
	}
}
