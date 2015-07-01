#include "ProjectTap.h"
#include "Graph.h"
#include "GVertex.h"
#include <assert.h>

AGraph::AGraph()
{
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UStaticMesh> edgeMesh(TEXT("/Game/Models/EdgeVisualizer"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> vertexMesh(TEXT("/Game/Models/VertexVisualizer"));

	edgeStaticMesh = edgeMesh.Object;
	vertexStaticMesh = vertexMesh.Object;
}

void AGraph::Init()
{
	if (edgeMatrix.Num() == 0)
	{
		edgeMatrix.SetNum(MAX_SIZE);

		for (size_t i = 0; i < MAX_SIZE; i++)
		{
			for (size_t j = 0; j < MAX_SIZE; j++)
			{
				if (j == 0)
				{
					edgeMatrix[i].vertex.SetNum(MAX_SIZE);
				}

				edgeMatrix[i].vertex[j] = NONEDGE;
			}
		}
	}

	if (mark.Num() == 0)
	{
		mark.SetNum(MAX_SIZE);
	}

	generateEdges();
}

void AGraph::PostLoad()
{
	Super::PostLoad();
	Init();
}

void AGraph::BeginPlay()
{
	Super::BeginPlay();
}
#if WITH_EDITOR
void AGraph::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		auto name = PropertyChangedEvent.Property->GetName();
		if (name.Equals("clearRoutVisuals"))
		{
			clearRouteVisuals();
		}
		else if (name.Equals("clickToInitialize"))
		{
			Init();
		}
	}
}
#endif
void AGraph::clearRouteVisuals()
{
	for (auto mesh : edgeMeshes)
	{
		if (mesh != nullptr)
		{
			mesh->DestroyComponent();
		}
	}

	edgeMeshes.Empty();

	for (auto mesh : vertexMeshes)
	{
		if (mesh != nullptr)
		{
			mesh->DestroyComponent();
		}
	}

	vertexMeshes.Empty();
}


void AGraph::BeginDestroy()
{
	Super::BeginDestroy();

	if (GetWorld() != nullptr)
	{
		for (size_t i = 0; i < mark.Num(); i++)
		{
			if (mark[i] != nullptr)
			{
				auto cons = mark[i]->connections;
				for (size_t j = 0; j < cons.Num(); j++)
				{
					mark[i]->disconnectTo(cons[j]);
				}

				mark[i]->vertexIndex = -1;
			}
		}
	}
}

AGVertex* AGraph::next(int32 v, int32 v2)
{
	for (size_t i = v2 + 1; i < MAX_SIZE; ++i)
	{
		if (i != v && edgeMatrix[v].vertex[i] == EDGE)
		{
			return mark[i];
		}
	}

	return nullptr;
}

AGVertex* AGraph::getConnectedVertexByIndex(int32 vertexIndex, 
											int32 connectionIndex)
{
	if (vertexIndex == connectionIndex) return nullptr;

	auto hasEdge = edgeMatrix[vertexIndex].vertex[connectionIndex] == EDGE;
	return hasEdge ? mark[connectionIndex] : nullptr;
}

AGVertex* AGraph::first(int32 v)
{
	for (size_t i = 0; i < MAX_SIZE; i++)
	{
		if (i != v && edgeMatrix[v].vertex[i] == EDGE)
		{
			return mark[i];
		}
	}

	return nullptr;
}

AGVertex* AGraph::FindNearestVertexTo(const FVector& dragRay, 
									  const AGVertex* vertex,
									  const float thresholdSquared)
{
	AGVertex* closestVertex = nullptr;
	auto minRange = FLT_MAX;

	//find the closest vertex from all verts that are connected to this vertex
	auto goal = mark[vertex->vertexIndex];
	for (size_t i = 0; 
		 i < MAX_SIZE; 
		 goal = getConnectedVertexByIndex(vertex->vertexIndex, i++))
	{ 
		if (goal == nullptr || goal->vertexIndex == vertex->vertexIndex) continue;		

		auto goalDir = goal->GetActorLocation() - vertex->GetActorLocation();
		auto flatGoalDir = FVector(goalDir.X, goalDir.Y, 0.0f);
		auto dotProduct = FVector::DotProduct(dragRay, flatGoalDir);
		
		if (dotProduct > 0.0f)
		{
			auto degree = FMath::RadiansToDegrees(FMath::Acos(dotProduct));
			float dragDistance = (dragRay).SizeSquared();
			float mouseDistanceToGoal = FVector::Dist(goal->GetActorLocation(), dragRay + vertex->GetActorLocation());
			bool inRange = dragDistance > thresholdSquared && mouseDistanceToGoal < minRange;

			if (inRange)
			{
				closestVertex = goal;
				minRange = mouseDistanceToGoal;
			}
		}
	}

	return closestVertex;
}

void AGraph::setUndirectedEdge(int32 v1, int32 v2)
{
	if (v1 != v2)
	{
		edgeMatrix[v1].vertex[v2] = EDGE;
		edgeMatrix[v2].vertex[v1] = EDGE;
	}
}

void AGraph::deleteUndirectedEdge(int32 v1, int32 v2)
{
	if (v1 >= 0 && v2 >= 0)
	{
		edgeMatrix[v1].vertex[v2] = NONEDGE;
		edgeMatrix[v2].vertex[v1] = NONEDGE;
	}
}

bool AGraph::hasEdge(int32 v1, int32 v2)
{
	assert(index >= 0 && index < MAX_SIZE);
	return edgeMatrix[v1].vertex[v2] == EDGE;
}

void AGraph::deleteAllVertsConnectionsToVert(int32 v, int32 connectIndex)
{
	for (size_t i = 0; i < mark.Num(); i++)
	{
		bool canDelete = mark[i] != nullptr && mark[i]->connections[connectIndex] == mark[v]->vertexIndex;
		if (canDelete)
		{
			deleteUndirectedEdge(i, v);
		}
	}
}

void AGraph::generateEdges()
{
	for (size_t v = 0; v < mark.Num(); v++)
	{
		auto vertex = mark[v];

		if (vertex != nullptr)
		{
			for (size_t i = 0; i < vertex->connections.Num(); i++)
			{
				auto connectIndex = vertex->connections[i];
				bool validIndex = connectIndex >= 0 && connectIndex < MAX_SIZE && getVertex(connectIndex) != nullptr;
				if (validIndex)
				{
					setUndirectedEdge(vertex->vertexIndex, connectIndex);
				}
			}
		}
	}
}

AGVertex* AGraph::getVertex(int v)
{	
	if(v >= 0 && v < MAX_SIZE)
		return mark[v];
	else
		return nullptr;
}

void AGraph::MakeVertexOccupied(int32 v)
{
	if(v >= 0 && v < MAX_SIZE)
		mark[v]->hasTile = true;
}

bool AGraph::MoveTileFromTo(int32 from, int32 to)
{
	bool canMoveTo = mark[from]->hasTile && !mark[to]->hasTile;

	if (canMoveTo)
	{
		mark[from]->hasTile = false;
		mark[to]->hasTile = true;
	}

	return canMoveTo;
}

bool AGraph::IsVertexOccupied(int32 v)
{
	assert(index >= 0 && index < MAX_SIZE);
	return mark[v]->hasTile;
}

void AGraph::addVertex(AGVertex* vertex)
{
	bool containsVertex = false;

	//check if contains vertex
	for (size_t i = 0; i < mark.Num() && !containsVertex; i++)
	{
		containsVertex = mark[i] != nullptr && 
			vertex->vertexIndex == mark[i]->vertexIndex;
	}

	for (size_t i = 0; i < mark.Num() && !containsVertex; i++)
	{
		if (mark[i] == nullptr)
		{
			mark[i] = vertex;
			vertex->vertexIndex = i;
			break;
		}
	}
}

void AGraph::generateGraphRouteVisualization()
{
	clearRouteVisuals();

	unmarkAll();
	TArray<int32> stack;

	for (size_t i = 0; i < MAX_SIZE; i++)
	{
		DFS_makeVisualizers(stack, i);
	}
#if WITH_EDITOR
	Super::PostEditChange();
#endif
}


void AGraph::DFS_makeVisualizers(TArray<int32>& stack,
								 int32 vIndex)
{
	if (mark[vIndex] == nullptr || mark[vIndex]->visited) return;

	markVertex(vIndex);
	stack.Push(vIndex);
	//generate a visulaizer for an unvisited vertex 
	auto vertexMesh = makeVertexMeshForVertex(vIndex);
	vertexMeshes.Add(vertexMesh);

	for (auto v = first(vIndex);
		v != nullptr;
		v = next(vIndex, v->vertexIndex))
	{
		bool isBackEdge = v->visited && vIndex < v->vertexIndex;

		if (!v->visited || isBackEdge)
		{
			//generate an edge
			auto edgeMesh = makeEdgeMeshForEdge(vIndex, v->vertexIndex);
			initializeEdgeMesh(edgeMesh, mark[vIndex], v);
			edgeMeshes.Add(edgeMesh);

			if (!v->visited)
			{
				DFS_makeVisualizers(stack, v->vertexIndex);
			}
		}
	}

	stack.Pop();
}



UStaticMeshComponent* AGraph::makeEdgeMeshForEdge(int32 i, int32 j)
{
	auto name = FString("Edge").Append(FString::FromInt(i)).Append(FString::FromInt(j));
	auto edge = NewObject<UStaticMeshComponent>(this, *name);
	edge->AttachTo(RootComponent);
	edge->SetStaticMesh(edgeStaticMesh);

	return edge;
}

UStaticMeshComponent* AGraph::makeVertexMeshForVertex(int32 v)
{
	auto name = FString("vertex").Append(FString::FromInt(v));
	auto vertex = NewObject<UStaticMeshComponent>(this, *name);
	vertex->AttachTo(RootComponent);

	vertex->SetStaticMesh(vertexStaticMesh);
	vertex->SetWorldScale3D(FVector(40.0f, 40.0f, 1.0f));
	vertex->SetWorldLocation(mark[v]->GetActorLocation());

	return vertex;
}

void AGraph::initializeEdgeMesh(UStaticMeshComponent* edgeMesh,
								const AGVertex* v1,
								const AGVertex* v2)
{
	auto pos1 = v1->GetActorLocation();
	auto pos2 = v2->GetActorLocation();

	
	auto ray = pos2 - pos1;
	
	auto n_p1p2 = ray;
	n_p1p2.Z = 0;
	n_p1p2 = n_p1p2.GetSafeNormal();
	
	//tile is 40*40*1. The distance needs to substract half of the tile size
	auto totalOffsetDistance = 40.0f;
	//change start & end positions from the center of the vertex mesh
	//to the edges of the mesh to the right direction
	auto p1_offset = n_p1p2 * totalOffsetDistance;
	pos1 += p1_offset;
	pos2 += -p1_offset;
	auto newRay = pos2 - pos1;

	auto rotation = FRotationMatrix::MakeFromX(newRay);
	auto scaleX = FVector::Dist(pos1, pos2) * .5f ;
	auto scaleY = 40.0f;
	auto scaleZ = 1.0f;
	FMatrix transformMatrix = FScaleMatrix(FVector(scaleX, scaleY, scaleZ)) *
								rotation *
								FTranslationMatrix(pos1);

	edgeMesh->SetWorldTransform(FTransform(transformMatrix));
	edgeMesh->AddLocalOffset(FVector(scaleX, 0.0f, 0.0f));
}

void AGraph::unmarkAll()
{
	for (size_t i = 0; i < mark.Num(); i++)
	{
		if (mark[i] != nullptr)
		{
			mark[i]->visited = false;
		}
	}
}

void AGraph::markVertex(int32 v)
{
	if (mark[v] != nullptr)
	{
		mark[v]->visited = true;
	}
}

void AGraph::markVertex(AGVertex* vertex)
{
	if (vertex != nullptr)
	{
		vertex->visited = true;
	}
}


void AGraph::removeVertex(AGVertex* vertex)
{
	for (size_t i = 0; i < mark.Num(); i++)
	{
		if (mark[i] != nullptr && (mark[i]->vertexIndex == vertex->vertexIndex))
		{
			mark[i] = nullptr;
			break;
		}
	}

	//delete all edges involving this vertex
	for (size_t i = 0; i < mark.Num(); i++)
	{
		deleteUndirectedEdge(vertex->vertexIndex, i);
	}
}

AGraph::~AGraph()
{

}
