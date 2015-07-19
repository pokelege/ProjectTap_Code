// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <Array.h>
#include "GameFramework/Actor.h"
#include "GVertex.h"
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
	static const int32 MAX_SIZE;
	static const int32 NONEDGE;
	static const int32 EDGE;

	TArray<int32> visitsMarks;
public:
	//stores visualizers for all edges
	UPROPERTY( VisibleAnywhere , BlueprintReadOnly , Category = visualizer )
		TArray<UStaticMeshComponent*> edgeMeshes;

	//stores visualizers for all verts
	UPROPERTY( VisibleAnywhere , BlueprintReadOnly , Category = visualizer )
		TArray<UStaticMeshComponent*> vertexMeshes;

	UPROPERTY( VisibleAnywhere , BlueprintReadOnly , Category = vertices )
		TArray<FVertexArray> edgeMatrix;

	UPROPERTY( VisibleAnywhere , BlueprintReadOnly , Category = vertices )
		TArray<AGVertex*> mark;
private:
	UStaticMesh* vertexStaticMesh;
	UStaticMesh* edgeStaticMesh;

	int32 numVert = 0;
	int32 numEdge = 0;

public:
	UPROPERTY( EditAnywhere , BlueprintReadOnly , Category = graph )
		bool clearRoutVisuals;

	UPROPERTY( EditAnywhere , BlueprintReadOnly , Category = graph )
		bool clickToInitialize;

	AGraph();
	~AGraph();

	virtual void BeginPlay() override;

	void generateEdges();

	void PostLoad() override;

	inline int n(){ return numVert; }

	inline int e(){ return numEdge; }

	void addVertex(class AGVertex* vertex);

	void removeVertex(class AGVertex* vertex);

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

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;
#endif
	bool IsMatrixInitialized();

	bool IsVertexOccupied(int32 v);

	AGVertex* getVertex(int index);

	void BeginDestroy() override;
private:
		void Init();

		int genId();

		void setUndirectedEdge( int32 v1 , int32 v2 );

		void deleteUndirectedEdge( int32 v1 , int32 v2 );

		void deleteAllVertsConnectionsToVert( int32 v ,
											  int32 connectIndex );

		//the following methods will only be excuted inside editor


		void DFS_makeVisualizers( TArray<int32>& stack ,
								  int32 vIndex );

		UStaticMeshComponent* makeEdgeMeshForEdge( int32 i , int32 j );

		UStaticMeshComponent* makeVertexMeshForVertex( int32 j );

		void initializeEdgeMesh( UStaticMeshComponent* edgeMesh ,
								 const class AGVertex* v1 ,
								 const class AGVertex* v2 );

		class AGVertex* next( int32 v ,
							  int32 v2 );

		class AGVertex* first( int32 v );

		class AGVertex* getConnectedVertexByIndex( int32 vertexIndex ,
												   int32 connectionIndex );

		void unmarkAll();
		void markVertex( int32 v_index );
		void markVertex( class AGVertex* vertex );

		void clearRouteVisuals();
};
