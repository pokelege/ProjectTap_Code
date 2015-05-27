// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GVertex.generated.h"

UCLASS()
class PROJECTTAP_API AGVertex : public AActor
{
	GENERATED_BODY()

	friend class AGraph;
	const int32 MAX_NUM = 4;
	bool hasTile = false;

	void regenerateDebugArrows();
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		UStaticMeshComponent* debugMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		TArray<int32> connectTo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Tile)
		int32 vertexIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		bool clickToGetIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		bool clickToMakeArrows;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Tile)
		TArray<UArrowComponent*> debugArrows;

	virtual void PostLoad() override;

	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;

	void renerateGraphArrows();

	AGVertex();
	~AGVertex();
	
};
