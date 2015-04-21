// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Ramp.h"

const FName ARamp::RAMP_MESH_PATH = FName("/Game/Models/Ramp");

const FName ARamp::RAMP_CURVE_PATH = FName("/Game/Curves/Ramp");

ARamp::ARamp(): ATile(  )
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
	}

	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(*RAMP_CURVE_PATH.ToString());
	if(curve.Object != nullptr) rotationSequence = curve.Object;

	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(40.0f, 40.0f, 2.0f));
}

void ARamp::BeginPlay()
{
	Super::BeginPlay();
}

void ARamp::Tick(float DeltaTime)
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
		TileMesh->SetRelativeRotation(FRotator(curveValue, 0, 0));
	}

}

///<summary>
///DEPRECATED activate the tile to use ramp
///will do nothing
///</summary>
void ARamp::BoostBall()
{
// 	TArray< AActor * > OverlappingActors;
// 	BallTrigger->GetOverlappingActors(OverlappingActors, ABallPawn::StaticClass());
// 	if (OverlappingActors.Num())
// 	{
// 		ABallPawn* pawn = Cast<ABallPawn>(OverlappingActors[0]);
// 		if (pawn != nullptr)
// 		{
// 			pawn->AddVelocity(forceMultiplier * GetActorForwardVector());
// 			//deactivate();
// 		}
// 	}
}

void ARamp::activate()
{
	if(rotationSequence == nullptr || activated) return;
	Super::activate();
	time = 0;
}
