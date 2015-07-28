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
	ball->ballCollision->SetPhysicsLinearVelocity(jumpVelocity);
	ball->ballCollision->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
}

void AJumpTile::calculateImpulse()
{
	//h = 1/2*a*t^2
	//t = sqrt(2h/a)
	float t_up = FMath::Sqrt(2 * height  / -GetWorld()->GetGravityZ()); 

	float fall_height = GetActorLocation().Z + height - target->GetActorLocation().Z;
	float t_down = FMath::Sqrt(2 * fall_height / -GetWorld()->GetGravityZ());

	float t = t_up + t_down;

	//vf = vi + at
	//vf == 0
	//vi = -at
	float verticalVelocity = -GetWorld()->GetGravityZ() * t_up;
	auto dir = (target->GetActorLocation() - ball->GetActorLocation()).GetSafeNormal();

	auto startPos = FVector(ball->GetActorLocation().X, ball->GetActorLocation().Y, .0f);// GetActorLocation().Z);
	auto targetPos = FVector(target->GetActorLocation().X, target->GetActorLocation().Y, .0f);// target->GetActorLocation().Y);
	auto distance = FVector::Dist(targetPos, startPos);

	//d = 1/2 * a *t^2
	//a = 2d / t^2
	auto horizontalSpeed = distance / t;

	//impuls(change in momentum) = f * t
	dir.Z = 0.0f;
	auto horizontalVec = dir * horizontalSpeed;
	jumpVelocity = horizontalVec + FVector(.0f, .0f, verticalVelocity);
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
			ball->AddVelocity(newDir * ballLandingForceStrength, true);
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

OffsetInfo AJumpTile::getOffsetInfo()
{
	OffsetInfo data;
	data.offsetForCollision = FVector(0.0f, 0.0f, 20.0f);
	data.scaleForCollision = FVector(1.0f, 1.0f, 2.0f);
	data.offsetForCarryOn = FVector(0.0f, 0.0f, 40.0f);
	return data;
}


#if WITH_EDITOR
void AJumpTile::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		auto p = PropertyChangedEvent.Property;
		auto pName = p->GetName();

		//when currentEditorPathIndex property changes in editor
		//reset current moving tile's location to desinated node's location
		if (target != nullptr && pName.Equals(TEXT("height")))
		{
			target->height = height;
		}
		else if (pName.Equals("target"))
		{
			target->height = height;
			target->target = this;
		}
	}
}
#endif
