#include "ProjectTap.h"
#include "Graph.h"
#include "GVertex.h"
#include <assert.h>

AGraph::AGraph()
{
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

	if (edgeMatrix.Num() == 0)
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


void AGraph::BeginDestroy()
{
	Super::BeginDestroy();
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

//bool AGraph::IsMatrixInitialized()
//{
//	return isMatrixInitialized;
//}
//
//void AGraph::SetMatrixInitialized(bool init)
//{
//	isMatrixInitialized = init;
//}

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
		bool canDelete = mark[i] != nullptr && mark[i]->connectTo[connectIndex] == mark[v]->vertexIndex;
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
			for (size_t i = 0; i < vertex->MAX_NUM; i++)
			{
				auto connectIndex = vertex->connectTo[i];
				bool validIndex = connectIndex >= 0 && connectIndex < MAX_SIZE && getVertex(connectIndex) != nullptr;
				if (validIndex)
				{
					setUndirectedEdge(vertex->vertexIndex, connectIndex);
				}
			}
		}
	}
}

AGVertex* AGraph::getVertex(int index)
{	
	assert(index >= 0 && index < MAX_SIZE);
	return mark[index];
}

void AGraph::MakeVertexOccupied(int32 v)
{
	assert(index >= 0 && index < MAX_SIZE);
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
