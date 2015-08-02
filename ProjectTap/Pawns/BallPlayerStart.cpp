// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BallPlayerStart.h"

const GroundableInfo ABallPlayerStart::groundableInfo = GroundableInfo(FVector(0, 0, 92), true);
const GroundableInfo* ABallPlayerStart::GetGroundableInfo() const
{
	return &ABallPlayerStart::groundableInfo;
}