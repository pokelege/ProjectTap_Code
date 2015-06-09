// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GVertex.h"
#include "Graph.h"
#include "Engine/GameInstance.h"
#include "ProjectTapGameState.h"

AGVertex::AGVertex()
{
	PrimaryActorTick.bCanEverTick = false;
	connectTo.SetNum(4);
	debugMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	SetRootComponent(debugMesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Game/Models/Ball"));
	debugMesh->SetWorldScale3D(FVector(0.1f));
	debugMesh->SetStaticMesh(tempMesh.Object);
	debugMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	for (size_t i = 0; i < MAX_NUM; i++)
	{
		connectTo[i] = -1;
	}

	vertexIndex = -1;

	//initialize debug arrows
	for (size_t i = 0; i < MAX_NUM; i++)
	{
		auto name = FString("debugArrow").Append(FString::FromInt(i));
		debugArrows.Add(CreateDefaultSubobject<UArrowComponent>(*name));
	}

	for (size_t i = 0; i < MAX_NUM; i++)
	{
		debugArrows[i]->AttachTo(RootComponent);
	}

}

void AGVertex::regenerateDebugArrows()
{
	auto g = getGraph();

	for (size_t i = 0; i < MAX_NUM; i++)
	{
		auto vIndex = connectTo[i];
		auto arrow = debugArrows[i];
		bool indexValid = vIndex >= 0 && vIndex < g->MAX_SIZE && vIndex != vertexIndex;
		if (indexValid)
		{
			auto other = g->getVertex(vIndex);

			if (other != nullptr)
			{
				auto start = GetActorLocation();
				auto end = other->GetActorLocation();
				auto distance = FVector::Dist(start, end);
				auto dir = (end - start);				

				arrow->SetWorldLocation(start);
				auto rot = FRotationMatrix::MakeFromX(dir);
				arrow->SetWorldLocation(start);
				arrow->SetWorldRotation(FRotator(rot.ToQuat()));
				arrow->SetRelativeScale3D(FVector(distance / 8.0f, 10.0f, 10.0f));				
			}
			else
			{
				connectTo[i] = -1;
				arrow->SetRelativeScale3D(FVector(0.0f));
			}
		}
		else 
		{
			connectTo[i] = -1;
			arrow->SetRelativeScale3D(FVector(0.0f));
		}
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
	renerateGraphArrows();
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
		else if (pName.Equals(TEXT("connectTo")))
		{
			g->generateEdges();
			regenerateDebugArrows();
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
	if (GetWorld() != nullptr)
	{
		getGraph()->removeVertex(this);
	}
}

AGVertex::~AGVertex()
{

}

