// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GVertex.h"
#include "Graph.h"
#include "Engine/GameInstance.h"
#include "ProjectTapGameState.h"

AGVertex::AGVertex()
{
	PrimaryActorTick.bCanEverTick = false;
	debugMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	SetRootComponent(debugMesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Game/Models/Ball"));
	debugMesh->SetWorldScale3D(FVector(0.1f));
	debugMesh->SetStaticMesh(tempMesh.Object);
	debugMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	vertexIndex = -1;
	
}

#define printonscreen(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

void AGVertex::EditorKeyPressed(FKey Key,
	EInputEvent Event)
{
	Super::EditorKeyPressed(Key, Event);	

	bool isConnectOperation = Key.GetFName().IsEqual("Enter");
	bool isDeleteOperation = Key.GetFName().IsEqual("C");
	
	bool executeOperation = (isConnectOperation || isDeleteOperation) &&
							EInputEvent::IE_Pressed == Event;
	if (executeOperation)
	{
		TArray<AGVertex*> selectedActors;
		auto g = getGraph();
		if (g == nullptr) return;
		for (TActorIterator<AGVertex> itr(GetWorld()); itr; ++itr)
		{
			if (*itr != this && itr->IsSelected())
			{
				selectedActors.Add(*itr);
			}
		}
		
		for (auto v : selectedActors)
		{			
			auto validConnectionNum = connections.Num() < MAX_NUM && v->connections.Num() < MAX_NUM;			
			auto canConnect =
								vertexIndex != -1 &&
								v->vertexIndex != -1;

			if (isConnectOperation && validConnectionNum && canConnect) 
			{
				v->connectTo(vertexIndex);
				connectTo(v->vertexIndex);
			}
			else if (isDeleteOperation)
			{
				v->disconnectTo(vertexIndex);
				disconnectTo(v->vertexIndex);
			}

		}

	}
}

void AGVertex::connectTo(int32 v)
{
	auto g = getGraph();
	auto other = g->getVertex(v);

	bool connectionValid = other != nullptr &&
						   !connections.Contains(v) &&
						   v != vertexIndex;

	bool connectionExists = debugArrows.Find(v) != nullptr &&
							other != nullptr;

	bool remakeArrow = debugArrows.Find(v) == nullptr && other != nullptr && connections.Contains(v);

	if (connectionValid || remakeArrow)
	{
		auto start = GetActorLocation();
		auto end = other->GetActorLocation();
		auto distance = FVector::Dist(start, end);
		auto dir = (end - start);
	
		auto arrow = makeArrowToVertex(other->vertexIndex);
		arrow->SetWorldLocation(start);
		auto rot = FRotationMatrix::MakeFromX(dir);
		arrow->SetWorldLocation(start);
		arrow->SetWorldRotation(FRotator(rot.ToQuat()));
		arrow->SetRelativeScale3D(FVector(distance / 8.0f, 10.0f, 10.0f));
		 
		if (!remakeArrow)
		{
			connections.Add(other->vertexIndex);
			g->setUndirectedEdge(vertexIndex, other->vertexIndex);
		}
	}
	else if (connectionExists)
	{
		auto start = GetActorLocation();
		auto end = other->GetActorLocation();
		auto distance = FVector::Dist(start, end);
		auto dir = (end - start);
		auto arrow = debugArrows[v];
		arrow->SetWorldLocation(start);
		auto rot = FRotationMatrix::MakeFromX(dir);
		arrow->SetWorldLocation(start);
		arrow->SetWorldRotation(FRotator(rot.ToQuat()));
		arrow->SetRelativeScale3D(FVector(distance / 8.0f, 10.0f, 10.0f));
	}
}

UArrowComponent* AGVertex::makeArrowToVertex(int32 v)
{
	auto name = FString("debugArrow").Append(FString::FromInt(v));
	auto arrow = ConstructObject<UArrowComponent>(UArrowComponent::StaticClass(), this, *name);
	arrow->RegisterComponent();
	arrow->AttachTo(GetRootComponent());
	if (debugArrows.Find(v) != nullptr)
	{
		debugArrows[v]->DestroyComponent();
		debugArrows.Remove(v);
	}

	debugArrows.Add(v, arrow);

	return arrow;
}

void AGVertex::disconnectTo(int32 v)
{
	static int count = 0;
	auto g = getGraph();

	for (size_t i = 0; i < connections.Num(); i++)
	{
		if (connections[i] == v)
		{
			if (debugArrows.Find(v) != nullptr)
			{
				debugArrows[v]->SetRelativeScale3D(FVector(0.0f));
				debugArrows[v]->DestroyComponent();
				debugArrows.Remove(v);
			}

			g->deleteUndirectedEdge(vertexIndex, v);
			connections.Remove(v);
			break;
		}
	}

	Super::PostEditChange();
}

void AGVertex::regenerateDebugArrows()
{
	for (size_t i = 0; i < connections.Num(); i++)
	{
		auto vIndex = connections[i];
		connectTo(vIndex);
	}
}

void AGVertex::renerateGraphArrows()
{
	for (TActorIterator<AGVertex> v_itr(GetWorld()); v_itr; ++v_itr)
	{
		v_itr->regenerateDebugArrows();
	}
}


void AGVertex::BeginPlay()
{
	Super::BeginPlay();
	debugMesh->SetHiddenInGame(true);
}

AGraph* AGVertex::getGraph()
{
	AGraph* graph = nullptr;
	//find graph actor
	for (TActorIterator<AGraph> graph_itr(GetWorld()); graph_itr; ++graph_itr)
	{
		graph = *graph_itr;
	}

	return graph;
}

void AGVertex::SetOccupied(bool occupied)
{
	hasTile = occupied;
}

bool AGVertex::IsVertexOccupied()
{
	return hasTile;
}

void AGVertex::EditorApplyTranslation
(
const FVector & DeltaTranslation,
bool bAltDown,
bool bShiftDown,
bool bCtrlDown
) 
{
	Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
	auto g = getGraph();
	if (g != nullptr)
	{
		g->clearRouteVisuals();
	}
	renerateGraphArrows();

	if (bAltDown)
	{
		vertexIndex = -1;
	}
}


void AGVertex::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	auto w = GetWorld();
	auto g = getGraph();
	
	if (PropertyChangedEvent.Property != nullptr)
	{
		auto pName = PropertyChangedEvent.Property->GetName();

		if (pName.Equals(TEXT("clickUpdateAllNodes")))
		{
			for (TActorIterator<AGVertex> v_itr(GetWorld()); v_itr; ++v_itr)
			{
				g->addVertex(*v_itr);
			}
		}
		else if (pName.Equals(TEXT("clickToMakeGraph")))
		{
			g->generateEdges();
			g->generateGraphRouteVisualization();
			renerateGraphArrows();
		}
	}
}

void AGVertex::PostLoad()
{
	Super::PostLoad();
}

void AGVertex::BeginDestroy()
{
	Super::BeginDestroy();

	//make sure vertex being deleted when it is inside the editor
	//instead of when editor is closing 
	auto g = getGraph();
	if (GetWorld() != nullptr && getGraph() != nullptr)
	{
		for (size_t i = 0; i < connections.Num(); i++)
		{
			auto other = g->getVertex(connections[i]);
			if (other != nullptr)
			{
				other->disconnectTo(vertexIndex);
			}

			disconnectTo(connections[i]);
		}

	}
}

AGVertex::~AGVertex()
{

}

