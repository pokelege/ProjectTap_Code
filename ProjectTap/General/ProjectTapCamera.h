// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ProjectTapCamera.generated.h"

UCLASS()
class PROJECTTAP_API AProjectTapCamera : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
	class UProjectTapCameraComponent* camera = nullptr;
	// Sets default values for this actor's properties
	AProjectTapCamera();
};
