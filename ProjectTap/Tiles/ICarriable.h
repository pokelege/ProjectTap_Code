// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Core/Public/Math/Vector.h"
#include "Runtime/CoreUObject/Public/Interface.h"
#include "ICarriable.generated.h"

struct OffsetInfo
{
	FVector scaleForCollision;
	FVector offsetForCollision;
	FVector offsetForCarryOn;
};

/**
 * Any tile that needs to be carried by 
 * draggable tile has to implement this interface
 */
UINTERFACE(MinimalAPI)
class UCarriable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};


class ICarriable
{
	GENERATED_IINTERFACE_BODY()
public:
	virtual OffsetInfo getOffsetInfo();
};