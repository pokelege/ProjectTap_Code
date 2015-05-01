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
	float duration = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	float height = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	AActor* target = nullptr;
	
	AJumpTile();

	virtual void BeginPlay() override;

	virtual void activate() override;
	void calculatePhysics();
};
