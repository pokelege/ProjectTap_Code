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
	static const int32 MAX_NUM;
public:
	TMap<int32 , UArrowComponent*> debugArrows;
	UPROPERTY( VisibleAnywhere , BlueprintReadWrite , Category = Tile )
		TArray<int32> connections;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		UStaticMeshComponent* debugMesh = nullptr;
	UPROPERTY( VisibleAnywhere , BlueprintReadWrite , Category = Tile )
		int32 vertexIndex = -1;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		bool clickUpdateAllNodes;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		bool clickToMakeGraph;
private:
	bool visited = false;
	bool hasTile = false;
	bool isSelected = false;
public:

	virtual void PostLoad() override;

	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;
#if WITH_EDITOR
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
#endif
	class AGraph* getGraph();

	void renerateGraphArrows();

	void SetOccupied(bool occupied);

	bool IsVertexOccupied();

	AGVertex();
	~AGVertex();
private:
	void regenerateDebugArrows();

	void disconnectTo( int32 v );

	void connectTo( int32 v );

	UArrowComponent* makeArrowToVertex( int32 v );
};
