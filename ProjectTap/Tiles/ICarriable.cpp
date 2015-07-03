// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ICarriable.h"

UCarriable::UCarriable(class FObjectInitializer const &ObjectInitializer) : Super(ObjectInitializer)
{

}

OffsetInfo ICarriable::getOffsetInfo()
{
	return OffsetInfo();
}
