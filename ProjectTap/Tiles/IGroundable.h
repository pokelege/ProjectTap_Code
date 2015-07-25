// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Vector.h"
#include "Interface.h"
#include "IGroundable.generated.h"
struct GroundableInfo
{
	FVector offset = FVector(0); //Extra offset after setting to ground
	bool defaultGroundVisibility = true; //Ground visiblility
	GroundableInfo( FVector theOffset, bool theDefaultGroundVisibility = true )
	{
		this->offset = theOffset;
		this->defaultGroundVisibility = theDefaultGroundVisibility;
	}
};
/**
 *
 */
UINTERFACE( MinimalAPI )
class UGroundable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IGroundable
{
	GENERATED_IINTERFACE_BODY()
public:
	virtual const GroundableInfo* GetGroundableInfo() const;
};
