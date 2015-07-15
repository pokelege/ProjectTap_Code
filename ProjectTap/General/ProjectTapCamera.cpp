// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapCamera.h"
#include "ProjectTapCameraComponent.h"

// Sets default values
AProjectTapCamera::AProjectTapCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	camera = CreateDefaultSubobject<UProjectTapCameraComponent>( TEXT( "Camera" ) );
}
