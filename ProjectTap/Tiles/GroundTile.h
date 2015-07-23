// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GroundTile.generated.h"

UCLASS()
class PROJECTTAP_API AGroundTile : public AActor
{
	GENERATED_BODY()
private:
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Mesh )
	class UBoxComponent* Collision = nullptr;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Mesh )
	TArray<UStaticMeshComponent*> Meshes;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Mesh )
	class UStaticMesh* Mesh = nullptr;
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
#if WITH_EDITOR
	virtual void PostEditChangeProperty( FPropertyChangedEvent & PropertyChangedEvent ) override;
#endif
	private:
		void Generate(bool isEditor = false);
};
