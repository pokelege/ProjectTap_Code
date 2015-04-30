// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/BaseRampTile.h"
#include "JumpTile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTAP_API AJumpTile : public ABaseRampTile
{
	GENERATED_BODY()
	
	static const FName JUMP_MESH_PATH;
	FVector force;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	AActor* target;
	
	virtual void BeginPlay() override;

	virtual void activate() override;
	void calculatePhysics();
};
