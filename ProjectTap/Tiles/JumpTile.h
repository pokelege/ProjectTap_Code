// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/BaseRampTile.h"
#include "ICarriable.h"
#include "JumpTile.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API AJumpTile : public ABaseRampTile, public ICarriable
{
	GENERATED_BODY()

	static const FName JUMP_MESH_PATH;
	FVector jumpVelocity;
	bool isBallComing = false;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	float height = 400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	float ballLandingForceStrength = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	AJumpTile* target = nullptr;
	
	AJumpTile();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;
#endif

	void SetWaitForBall();

	OffsetInfo getOffsetInfo() override;

	virtual void BeginPlay() override;

	virtual void activate() override;
	void calculateImpulse();
	virtual void HighlightEdge() override;
	virtual void CancelHighlightEdge() override;
	virtual void HighlightTile() override;
	virtual void CancelHighlightTile() override;
};
