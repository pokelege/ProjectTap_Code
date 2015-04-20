// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Ramp.h"

const FName ARamp::RAMP_MESH_PATH = FName("/Game/Models/Ramp");

const FName ARamp::RAMP_TOP_MESH_PATH = FName("/Game/Models/RampTop");

const FName ARamp::RAMP_CURVE_PATH = FName("/Game/Curves/Ramp");

ARamp::ARamp(): ATile(  )
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*RAMP_MESH_PATH.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	TileMesh->SetRelativeLocation(FVector(0,0,-1), false, nullptr);

	if(TopMesh == nullptr)
	{
		TopMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Ramp top mesh" ) );
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> topMesh(*RAMP_TOP_MESH_PATH.ToString());
	TopMesh->SetStaticMesh(topMesh.Object);
	TopMesh->SetRelativeLocation(FVector(1,0,0), false, nullptr);
	TopMesh->AttachTo(this->GetRootComponent());
	TopMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TopMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TopMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(1,1,1), false);
		BoxCollision->SetRelativeLocation(FVector(0,0,-1), false, nullptr);
	}

	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(*RAMP_CURVE_PATH.ToString());
	if(curve.Object != nullptr) rotationSequence = curve.Object;
// 	if(!BallTrigger)
// 	{
// 		BallTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Ball trigger"));
// 		BallTrigger->SetBoxExtent(FVector(1,1,1), false);
// 		FTransform transform = BallTrigger->GetRelativeTransform();
// 		transform.SetLocation(FVector(0,0,1));
//
// 		BallTrigger->SetRelativeTransform(transform);
// 		BallTrigger->AttachTo(this->GetRootComponent());
// 	}

	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(40.0f, 40.0f, 80.0f));
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
// 		TArray< AActor * > OverlappingActors;
// 		BallTrigger->GetOverlappingActors(OverlappingActors, ABallPawn::StaticClass());
// 		if (OverlappingActors.Num())
// 		{
// 			ABallPawn* pawn = Cast<ABallPawn>(OverlappingActors[0]);
// 			if (pawn != nullptr)
// 			{
// 				pawn->AddVelocity(forceMultiplier * GetActorForwardVector());
// 			}
// 		}
		float minTime, maxTime;
		rotationSequence->GetTimeRange(minTime, maxTime);
		if((time += DeltaTime) >= maxTime)
		{
			time = maxTime;
			deactivate();
		}
		float curveValue = rotationSequence->GetFloatValue(time);
		TopMesh->SetRelativeRotation(FRotator(curveValue, 0, 0));
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
