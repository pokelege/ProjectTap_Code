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

	Graph::GetInstance()->addVertex(this);

	for (size_t i = 0; i < MAX_NUM; i++)
	{
		connectTo[i] = -1;
	}

#ifdef DEBUG_ON
	//initialize debug arrows
	for (size_t i = 0; i < debugArrows.Num(); i++)
	{
		debugArrows[i] = CreateDefaultSubobject<UArrowComponent>(TEXT("graphArrow"));
	}

#endif
}

#ifdef DEBUG_ON
void AGVertex::regenerateDebugArrows()
{
	auto g = Graph::GetInstance();

	for (size_t i = 0; i < MAX_NUM; i++)
	{
		auto vIndex = connectTo[i];

		if (vIndex != -1)
		{
			auto other = g->getVertex(vIndex);
			auto start = GetActorLocation();
			auto end = other->GetActorLocation();
			auto distance = FVector::Dist(start, end);
			auto dir = (end - start).GetSafeNormal();

			auto arrow = debugArrows[i];
			auto arrowLocation = start + dir * distance * 0.5f;
			arrow->SetWorldLocation(arrowLocation);
			auto trans = FRotationMatrix::MakeFromX(dir);
			trans.SetOrigin(arrowLocation);
			arrow->SetWorldTransform(FTransform(trans));
			arrow->SetRelativeScale3D(FVector(distance * 0.5f, 0.0f, 0.0f));
		}
	}
}
#endif

void AGVertex::BeginPlay()
{
	Super::BeginPlay();
	debugMesh->SetHiddenInGame(true);
}


void AGVertex::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	
	regenerateDebugArrows();
}

void AGVertex::BeginDestroy()
{
	Super::BeginDestroy();

	Graph::GetInstance()->removeVertex(this);
}


AGVertex::~AGVertex()
{

}

