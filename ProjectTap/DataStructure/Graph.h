// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <Array.h>
#include "GameFramework/Actor.h"
#include "Graph.generated.h"

UCLASS()
class PROJECTTAP_API AGraph : public AActor
{
	GENERATED_BODY()

	int32** matrix;
	friend class AGVertex;
	const int32 MAX_SIZE = 25;

private:
	const int32 NONEDGE = 0;
	const int32 EDGE = 1;

	TArray<class AGVertex*> mark;
	int32 numVert = 0;
	int32 numEdge = 0;

	void Init();

	int genId();

	void setUndirectedEdge(int32 v1, int32 v2);

	void deleteUndirectedEdge(int32 v1, int32 v2);

	void deleteAllVertsConnectionsToVert(int32 v, int32 connectIndex);

public:

	AGraph();
	~AGraph();

	void generateEdges();

	inline int n(){ return numVert; }

	inline int e(){ return numEdge; }

	void addVertex(AGVertex* vertex);

	void removeVertex(AGVertex* vertex);

	bool hasEdge(int32 v1, int32 v2);

	AGVertex* FindEdgeInDirection(const FVector& dir);

	void MakeVertexOccupied(int32 v);

	bool MoveTileFromTo(int32 from, int32 to);

	bool IsVertexOccupied(int32 v);

	AGVertex* getVertex(int index);

	void BeginDestroy() override;
	
};
