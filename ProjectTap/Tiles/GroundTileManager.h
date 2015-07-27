// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GroundTileManager.generated.h"

UCLASS()
class PROJECTTAP_API AGroundTileManager : public AActor
{
	GENERATED_BODY()

private:
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Mesh )
	class UBoxComponent* Collision = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Size )
		int32 NumTilesX = 0;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Size )
		int32 NumTilesY = 0;

		const float MeshScaleX = 40.0f;
		const float MeshScaleY = 40.0f;
		const float MeshScaleZ = 80.0f;
		UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Generate)
		bool ApplyProperties_Button;
	// Sets default values for this actor's properties
	AGroundTileManager();
#if WITH_EDITOR
	virtual void PostEditChangeProperty( FPropertyChangedEvent & PropertyChangedEvent ) override;
#endif
private:
	void Generate( );
};
