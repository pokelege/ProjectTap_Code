#include "ProjectTap.h"
#include "Graph.h"
#include "GVertex.h"
#include <assert.h>

bool Graph::isInitialized = false;
Graph* Graph::graph = nullptr;

Graph::Graph()
{

}

void Graph::Init()
{
	matrix = new int32*[MAX_SIZE];

	mark.SetNum(MAX_SIZE);

	for (size_t i = 0; i < MAX_SIZE; i++)
	{
		matrix[i] = new int32[MAX_SIZE];
	}

	for (size_t i = 0; i < MAX_SIZE; i++)
	{
		for (size_t j = 0; j < MAX_SIZE; j++)
		{
			matrix[i][j] = NONEDGE;
		}
	}
}

Graph* Graph::GetInstance()
{
	if (isInitialized)
	{
		return graph;
	}
	else
	{
		isInitialized = true;
		graph = new Graph;
		graph->Init();
		return graph;
	}

}

void Graph::Destroy()
{
	for (size_t i = 0; i < MAX_SIZE; i++)
	{
		delete[]matrix[i];
	}

	delete[]matrix;
	matrix = nullptr;

	delete graph;
	graph = nullptr;

	numVert = 0;
	numEdge = 0;
	isInitialized = false;
}


AGVertex* Graph::FindEdgeInDirection(const FVector& dir)
{

	return nullptr;
}

void Graph::setUndirectedEdge(int32 v1, int32 v2)
{
	if (v1 != v2)
	{
		matrix[v1][v2] = EDGE;
		matrix[v2][v1] = EDGE;
	}
}

void Graph::deleteUndirectedEdge(int32 v1, int32 v2)
{
	if (v1 >= 0 && v2 >= 0)
	{
		matrix[v1][v2] = NONEDGE;
		matrix[v2][v1] = NONEDGE;
	}
}

bool Graph::hasEdge(int32 v1, int32 v2)
{
	assert(index >= 0 && index < MAX_SIZE);
	return matrix[v1][v2] == EDGE;
}

void Graph::generateEdges()
{
	for (size_t v = 0; v < mark.Num(); v++)
	{
		auto vertex = mark[v];

		if (vertex != nullptr)
		{
			for (size_t i = 0; i < vertex->MAX_NUM; i++)
			{
				auto connectIndex = vertex->connectTo[i];

				if (connectIndex >= 0 && connectIndex < MAX_SIZE)
				{
					setUndirectedEdge(vertex->vertexIndex, connectIndex);
				}
			}
		}
	}
}

AGVertex* Graph::getVertex(int index)
{	
	assert(index >= 0 && index < MAX_SIZE);

	return mark[index];
}

void Graph::MakeVertexOccupied(int32 v)
{
	assert(index >= 0 && index < MAX_SIZE);
	mark[v]->hasTile = true;
}

bool Graph::MoveTileFromTo(int32 from, int32 to)
{
	bool canMoveTo = mark[from]->hasTile && !mark[to]->hasTile;

	if (canMoveTo)
	{
		mark[from]->hasTile = false;
		mark[to]->hasTile = true;
	}

	return canMoveTo;
}

bool Graph::IsVertexOccupied(int32 v)
{
	assert(index >= 0 && index < MAX_SIZE);
	return mark[v]->hasTile;
}

void Graph::addVertex(AGVertex* vertex)
{
	bool containsVertex = false;
	//check if contains vertex
	for (size_t i = 0; i < mark.Num() && !containsVertex; i++)
	{
		containsVertex = mark[i] != nullptr && vertex == mark[i];
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

void Graph::removeVertex(AGVertex* vertex)
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

Graph::~Graph()
{

}
