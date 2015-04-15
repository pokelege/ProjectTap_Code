// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Ramp.h"

ARamp::ARamp(): ATile(  )
{
	PrimaryActorTick.bCanEverTick = true;
	FName path("/Game/Models/Ramp");
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
// 	TileMesh->SetRelativeTransform(
// 		TileMesh->GetRelativeTransform()
// 		.SetLocation(
// 			FVector(0,1,1)));
	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(1,1,1), false);
	}
	if(!BallTrigger)
	{
		BallTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Ball trigger"));
		BallTrigger->SetBoxExtent(FVector(1,1,1), false);
		FTransform transform = BallTrigger->GetRelativeTransform();
		transform.SetLocation(FVector(0,0,2));

		BallTrigger->SetRelativeTransform(transform);
		BallTrigger->AttachTo(BoxCollision);
	}

	GetRootPrimitiveComponent()->SetWorldScale3D(FVector(20.0f, 20.0f, 80.0f));
}

void ARamp::BeginPlay()
{
	Super::BeginPlay();	//todo Set transforms
	//originalMatrix = this->GetTransform().ToMatrixWithScale();
	//time = 0;
	//reverse = false;
	//float nothing = 0;
	//rotationSequence->GetTimeRange(nothing, duration);
	//BallCollision->SetRelativeLocation(FVector(0,0,RootComponent->GetRelativeTransform().GetScale3D().Z * 2));
	//activate();
}

void ARamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//todo animation
// 	if(activated)
// 	{
// 		if(reverse) time-=DeltaTime;
// 		else time+=DeltaTime;
// 		if(time>=duration)
// 		{
// 			reverse = true;
// 			time = duration;
// 		}
// 		else if(time<=0)
// 		{
// 			reverse = false;
// 			time = 0;
// 			deactivate();
// 		}
// 		FMatrix transform = FTranslationMatrix::Make(pivot)
// 		* FRotationMatrix::Make(FRotator::FRotator(0,0,rotationSequence->GetFloatValue(time)))
// 		* FTranslationMatrix::Make(-pivot)
// 		* originalMatrix;
// 	}

	if(activated)
	{
		if(BallTrigger->IsOverlappingActor(ball))
		{
			ball->AddVelocity(forceMultiplier * moveDirection );
		}
	}
}