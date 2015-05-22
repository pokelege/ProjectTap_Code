// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GVertex.generated.h"

UCLASS()
class PROJECTTAP_API AGVertex : public AActor
{
	GENERATED_BODY()


	friend class Graph;
	const int32 MAX_NUM = 4;
	bool hasTile = false;

#ifdef DEBUG_ON
	void regenerateDebugArrows();
#else
	void regenerateDebugArrows(){}
#endif

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		UStaticMeshComponent* debugMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		TArray<int32> connectTo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Tile)
		int32 vertexIndex;
#ifdef DEBUG_ON
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Tile)
		TArray<UArrowComponent*> debugArrows;

#endif

	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;

	AGVertex();
	~AGVertex();
	
};
