// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Ramp.h"

ARamp::ARamp( const FObjectInitializer& initializer ): ATile( initializer )
{
	PrimaryActorTick.bCanEverTick = true;
	FName path("/Game/Models/Ramp");
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(1,1,1), false);
	}
	if(!BallCollision)
	{
		BallCollision = initializer.CreateDefaultSubobject<UBoxComponent>(TileMesh, TEXT("BallCollision"));
		BallCollision->AttachTo(RootComponent);
		BallCollision->SetBoxExtent(FVector(1,1,1), false);
		BallCollision->SetRelativeLocation(FVector(0,0,2));
	}
}

void ARamp::BeginPlay()
{
	Super::BeginPlay();	//todo Set transforms
	//originalMatrix = this->GetTransform().ToMatrixWithScale();
	//time = 0;
	//reverse = false;
	//float nothing = 0;
	//rotationSequence->GetTimeRange(nothing, duration);
	BallCollision->SetRelativeLocation(FVector(0,0,RootComponent->GetRelativeTransform().GetScale3D().Z));
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
		if(BallCollision->IsOverlappingActor(ball))
		{
			ball->AddVelocity(forceMultiplier * moveDirection );
		}
	}
}