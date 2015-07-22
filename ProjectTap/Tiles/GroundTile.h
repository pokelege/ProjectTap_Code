// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GroundTile.generated.h"

UCLASS()
class PROJECTTAP_API AGroundTile : public AActor
{
	GENERATED_BODY()
private:
	class UBoxComponent* Collision = nullptr;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Size )
		TArray<FVector> IgnoredMeshes;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Size , meta = ( UIMin = "1.0" , UIMax = "10.0" ) )
		int32 NumTilesX = 1;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Size , meta = ( UIMin = "1.0" , UIMax = "10.0" ) )
		int32 NumTilesY = 1;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Size )
		float MeshScaleX = 40.0f;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Size )
		float MeshScaleY = 40.0f;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Size )
		float MeshScaleZ = 80.0f;
	// Sets default values for this actor's properties
	AGroundTile();
	private:
		void Generate();
};
