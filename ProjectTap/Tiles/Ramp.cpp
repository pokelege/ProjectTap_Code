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

	if(boxTrigger == nullptr)
	{
		boxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Ramp box trigger"));
		boxTrigger->AttachTo(this->GetRootComponent());
		boxTrigger->SetBoxExtent(FVector(0.5f,0.5f,2.0f), false);
	}

	boxTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxTrigger->bGenerateOverlapEvents = true;
	boxTrigger->SetNotifyRigidBodyCollision(false);
	boxTrigger->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	boxTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	pawnIn.BindUFunction(this, "OnBeginTriggerOverlap");
	boxTrigger->OnComponentBeginOverlap.Add(pawnIn);
	pawnOut.BindUFunction(this, "OnEndTriggerOverlap");
	boxTrigger->OnComponentEndOverlap.Add(pawnOut);
	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(*RAMP_CURVE_PATH.ToString());
	if(curve.Object != nullptr) rotationSequence = curve.Object;

	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(40.0f, 40.0f, 2.0f));

	forceMultiplier = 1000.0f;
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
	if(rotationSequence == nullptr || ball == nullptr || activated) return;
	Super::activate();
	time = 0;
	switch(rotationDirection)
	{
		case Direction::XPlus:
			ball->AddVelocity(forceMultiplier * FVector(1,0,0));
			break;
		case Direction::xMinus:
			ball->AddVelocity(forceMultiplier * FVector(-1,0,0));
			break;
		case Direction::YPlus:
			ball->AddVelocity(forceMultiplier * FVector(0,1,0));
			break;
		case Direction::yMinus:
			ball->AddVelocity(forceMultiplier * FVector(0,-1,0));
			break;
	}
}

void ARamp::OnBeginTriggerOverlap(AActor* OtherActor,
						   UPrimitiveComponent* OtherComp,
						   int32 OtherBodyIndex,
						   bool bFromSweep,
						   const FHitResult & SweepResult)
{
	ball = Cast<ABallPawn>(OtherActor);
}
void ARamp::OnEndTriggerOverlap(AActor* OtherActor,
						 UPrimitiveComponent* OtherComp,
						 int32 OtherBodyIndex)
{
	if(Cast<ABallPawn>(OtherActor) != nullptr) ball = nullptr;
}
