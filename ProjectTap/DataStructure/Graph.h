// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <Array.h>
#include "GVertex.h"
#include "GameFramework/Actor.h"
#include "Graph.generated.h"

USTRUCT()
struct FVertexArray
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = vertices)
	TArray<int32> vertex;
};

UCLASS()
class PROJECTTAP_API AGraph : public AActor
{
	GENERATED_BODY()
	friend class AGVertex;
	const int32 MAX_SIZE = 25;

private:
	TArray<int32> visitsMarks;
	UStaticMesh* vertexStaticMesh;
	UStaticMesh* edgeStaticMesh;

	const int32 NONEDGE = -1;
	const int32 EDGE = 1;

	int32 numVert = 0;
	int32 numEdge = 0;

	void Init();

	int genId();

	void setUndirectedEdge(int32 v1, int32 v2);

	void deleteUndirectedEdge(int32 v1, int32 v2);

	void deleteAllVertsConnectionsToVert(int32 v, 
										 int32 connectIndex);

	//the following methods will only be excuted inside editor


	void DFS_makeVisualizers(TArray<int32> stack,
							 int32 vIndex);

	UStaticMeshComponent* makeEdgeMeshForEdge(int32 i, int32 j);

	UStaticMeshComponent* makeVertexMeshForVertex(int32 j);

	void initializeEdgeMesh(UStaticMeshComponent* edgeMesh,
							const AGVertex* v1,
							const AGVertex* v2);
	
	AGVertex* next(int32 v, int32 v2);

	AGVertex* first(int32 v);

	AGVertex* getConnectedVertexByIndex(int32 vertexIndex, 
										int32 connectionIndex);

	void unmarkAll();
	void markVertex(int32 v_index);
	void markVertex(AGVertex* vertex);

public:

	AGraph();
	~AGraph();

	//stores visualizers for all edges
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = visualizer)
	TArray<UStaticMeshComponent*> edgeMeshes;

	//stores visualizers for all verts
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = visualizer)
	TArray<UStaticMeshComponent*> vertexMeshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = vertices)
	TArray<FVertexArray> edgeMatrix;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = vertices)
	TArray<AGVertex*> mark;

	void generateEdges();

	void PostLoad() override;

	inline int n(){ return numVert; }

	inline int e(){ return numEdge; }

	void addVertex(AGVertex* vertex);

	void removeVertex(AGVertex* vertex);

	bool hasEdge(int32 v1, int32 v2);

	//find the nearest vertex relative to drag tile's current position
	//@Param dragRay: the ray from drag tile's position to camera ray position
	//@Param thresholdSquared: minimum distance required to find a vertex
	//@return: will return nullptr if there no match
	AGVertex* FindNearestVertexTo(const FVector& dragRay,
								  const AGVertex* vertex,
								  const float thresholdSquared = 9000.0f);	

	void MakeVertexOccupied(int32 v);

	bool MoveTileFromTo(int32 from, 
					    int32 to);

	void generateGraphRouteVisualization();

	void SetMatrixInitialized(bool init);

	bool IsMatrixInitialized();

	bool IsVertexOccupied(int32 v);

	AGVertex* getVertex(int index);

	void BeginDestroy() override;
	
};
