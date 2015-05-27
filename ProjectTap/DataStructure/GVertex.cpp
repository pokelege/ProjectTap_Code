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

void AGVertex::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	auto w = GetWorld();
	auto g = getGraph();
	
	if (PropertyChangedEvent.Property != nullptr)
	{
		auto pName = PropertyChangedEvent.Property->GetName();

		if (pName.Equals(TEXT("clickToGetIndex")))
		{
			g->addVertex(this);
		}
		else if (pName.Equals(TEXT("connectTo")))
		{
			g->generateEdges();
			regenerateDebugArrows();
		}
		else if (pName.Equals(TEXT("clickToMakeArrows")))
		{
			renerateGraphArrows();
		}
	}
}

void AGVertex::PostLoad()
{
	Super::PostLoad();
	vertexIndex = -1;
	//Graph::GetInstance()->addVertex(this);
}


void AGVertex::BeginDestroy()
{
	Super::BeginDestroy();

	//Graph::GetInstance()->removeVertex(this);
}


AGVertex::~AGVertex()
{

}

