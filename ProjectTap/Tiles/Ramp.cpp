// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Ramp.h"


void ARamp::BeginPlay()
{
	Super::BeginPlay();
	originalMatrix = this->GetTransform().ToMatrixWithScale();
}

void ARamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if(activated)
	{
		if(rotationSequence->IsAtStart() && rotationSequence->IsInReverse())
		{
			deactivate();
		}
		else if(rotationSequence->IsAtEnd())
		{
			rotationSequence->PlayReverse();
		}
	}*/

	//FMatrix transform = FTranslationMatrix::Make(pivot) * FRotationMatrix::Make(FRotator::FRotator(0,0,rotationSequence->GetLerp())) * FTranslationMatrix::Make(-pivot) * originalMatrix;
	
}
void ARamp::activate()
{
	if(activated) return;
	Super::activate();
	//rotationSequence->Play();
}
