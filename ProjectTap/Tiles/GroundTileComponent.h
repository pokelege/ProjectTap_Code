// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "GroundTileComponent.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API UGroundTileComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
private:
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Class )
	UClass* TileClass = nullptr;
	UGroundTileComponent();
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;
#endif
};
