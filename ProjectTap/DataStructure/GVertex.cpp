// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GVertex.h"
#include "Graph.h"




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
	auto g = Graph::GetInstance();

	for (size_t i = 0; i < MAX_NUM; i++)
	{
		auto vIndex = connectTo[i];
		bool indexValid = vIndex >= 0 && vIndex < g->MAX_SIZE && vIndex != vertexIndex;
		if (indexValid)
		{
			auto other = g->getVertex(vIndex);
			auto start = GetActorLocation();
			auto end = other->GetActorLocation();
			auto distance = FVector::Dist(start, end);
			auto dir = (end - start);

			auto arrow = debugArrows[i];
			arrow->SetWorldLocation(start);
			auto rot = FRotationMatrix::MakeFromX(dir);			
			arrow->SetWorldLocation(start);
			arrow->SetWorldRotation(FRotator(rot.ToQuat()));
			arrow->SetWorldScale3D(FVector(80.0f, 10.0f, 10.0f));
			//FTransform transform;
			//transform.SetLocation(arrowLocation);
			//transform.SetRotation(rot.ToQuat());
			//arrow->SetWorldTransform(transform);
		}
		else 
		{
			connectTo[i] = -1;
		}
	}
}

void AGVertex::BeginPlay()
{
	Super::BeginPlay();
	debugMesh->SetHiddenInGame(true);
}


void AGVertex::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		auto pName = PropertyChangedEvent.Property->GetName();

		if (pName.Equals(TEXT("clickToGetIndex")))
		{
			Graph::GetInstance()->addVertex(this);
		}
		else if (pName.Equals(TEXT("clickToMakeArrows")))
		{
			regenerateDebugArrows();
		}
	}
}

void AGVertex::PostLoad()
{
	Super::PostLoad();

	Graph::GetInstance()->addVertex(this);

}


void AGVertex::BeginDestroy()
{
	Super::BeginDestroy();

	Graph::GetInstance()->removeVertex(this);
}


AGVertex::~AGVertex()
{

}

