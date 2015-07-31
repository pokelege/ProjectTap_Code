// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GroundTileManager.generated.h"

UCLASS(meta = (ToolTip = "The ground tile manager to manage ground tiles"))
class PROJECTTAP_API AGroundTileManager : public AActor
{
	GENERATED_BODY()

private:
public:
	UPROPERTY()
	class UBoxComponent* Collision = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Size , meta = ( ToolTip = "Number of X tiles in local space." ) )
		int32 NumTilesX = 0;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Size , meta = ( ToolTip = "Number of Y tiles in local space." ) )
		int32 NumTilesY = 0;

	const float MeshScaleX = 40.0f;
	const float MeshScaleY = 40.0f;
	const float MeshScaleZ = 80.0f;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Generate , meta = ( ToolTip = "Button to generate the ground tiles. (Enter)" ) )
		bool ApplyProperties_Button;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Children , meta = ( ToolTip = "Enable destroying child actors." ) )
		bool DestroyActorsWithGroundManager = true;
	// Sets default values for this actor's properties
	AGroundTileManager();
	virtual void Destroyed();
#if WITH_EDITOR
	virtual void EditorKeyPressed( FKey Key ,
								   EInputEvent Event ) override;
	virtual void PostEditChangeProperty( FPropertyChangedEvent & PropertyChangedEvent ) override;
#endif
private:
	void Generate();
};
