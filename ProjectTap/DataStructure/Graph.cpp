#include "ProjectTap.h"
#include "Graph.h"
#include "GVertex.h"
#include <assert.h>

AGraph::AGraph()
{
	Init();
}

void AGraph::Init()
{
	matrix.SetNum(MAX_SIZE);

	for (size_t i = 0; i < MAX_SIZE; i++)
	{
		for (size_t j = 0; j < MAX_SIZE; j++)
		{
			if (j == 0)
			{
				matrix[i].vertex.SetNum(MAX_SIZE);
			}

			matrix[i].vertex[j] = NONEDGE;
		}
	}
}

void AGraph::BeginDestroy()
{
	Super::BeginDestroy();
}


AGVertex* AGraph::FindEdgeInDirection(const FVector& dir)
{
	return nullptr;
}

void AGraph::setUndirectedEdge(int32 v1, int32 v2)
{
	if (v1 != v2)
	{
		matrix[v1].vertex[v2] = EDGE;
		matrix[v2].vertex[v1] = EDGE;
	}
}

void AGraph::deleteUndirectedEdge(int32 v1, int32 v2)
{
	if (v1 >= 0 && v2 >= 0)
	{
		matrix[v1].vertex[v2] = NONEDGE;
		matrix[v2].vertex[v1] = NONEDGE;
	}
}

bool AGraph::hasEdge(int32 v1, int32 v2)
{
	assert(index >= 0 && index < MAX_SIZE);
	return matrix[v1].vertex[v2] == EDGE;
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
					if (mark[connectIndex]->connectTo[i] == -1)
					{
						mark[connectIndex]->connectTo[i] = vertex->vertexIndex;
					}
					else
					{
						deleteAllVertsConnectionsToVert(v, connectIndex);
					}
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
