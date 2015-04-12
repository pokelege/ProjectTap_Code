// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Ramp.h"


void ARamp::BeginPlay()
{
	Super::BeginPlay();
	originalMatrix = this->GetTransform().ToMatrixWithScale();
	time = 0;
	reverse = false;
	float nothing = 0;
	rotationSequence->GetTimeRange(nothing, duration);
	activate();
}

void ARamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(activated)
	{
		if(reverse) time-=DeltaTime;
		else time+=DeltaTime;
		if(time>=duration)
		{
			reverse = true;
			time = duration;
		}
		else if(time<=0)
		{
			reverse = false;
			time = 0;
			deactivate();
		}
		FMatrix transform = FTranslationMatrix::Make(pivot)
		* FRotationMatrix::Make(FRotator::FRotator(0,0,rotationSequence->GetFloatValue(time)))
		* FTranslationMatrix::Make(-pivot)
		* originalMatrix;
	}
	
}