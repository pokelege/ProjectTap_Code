// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Map.h"
#include "GameFramework/Actor.h"
#include "GVertex.generated.h"

UCLASS()
class PROJECTTAP_API AGVertex : public AActor
{
	GENERATED_BODY()

	friend class AGraph;
	const int32 MAX_NUM = 4;
	bool visited = false;
	bool hasTile = false;
	bool isSelected = false;

	void regenerateDebugArrows();

	void disconnectTo(int32 v);

	void connectTo(int32 v);
	
	UArrowComponent* makeArrowToVertex(int32 v);
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		UStaticMeshComponent* debugMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Tile)
		TArray<int32> connections;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Tile)
		int32 vertexIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		bool clickUpdateAllNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		bool clickToMakeGraph;

	TMap<int32, UArrowComponent*> debugArrows;

	virtual void PostLoad() override;

	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	virtual void EditorKeyPressed(FKey Key,
								  EInputEvent Event) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;

	virtual void EditorApplyTranslation
	(
		const FVector & DeltaTranslation,
		bool bAltDown,
		bool bShiftDown,
		bool bCtrlDown
	) override;
	
	class AGraph* getGraph();

	void renerateGraphArrows();

	void SetOccupied(bool occupied);

	bool IsVertexOccupied();

	AGVertex();
	~AGVertex();
	
};
