// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "IGroundable.h"

UGroundable::UGroundable( class FObjectInitializer const &ObjectInitializer ) : Super( ObjectInitializer )
{

}

const GroundableInfo* IGroundable::GetGroundableInfo() const
{
	return nullptr;
}