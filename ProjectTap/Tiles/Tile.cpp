// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Tile.h"
#include "EmptyComponent.h"
// Sets default values
ATile::ATile()
{
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Tile mesh" ) );

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Tile collision"));
	BoxCollision->bGenerateOverlapEvents = true;
	this->SetRootComponent(BoxCollision);

	TileMesh->AttachTo(this->GetRootComponent());
	TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->bGenerateOverlapEvents = false;
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollision->SetNotifyRigidBodyCollision(true);
	CancelHighlight();
}

void ATile::activate()
{
	if (IsEnabled())
	{
		activated = true;
	}
}

void ATile::deactivate()
{
	activated = false;
}

bool ATile::isActivated()
{
	return activated;
}

void ATile::Enable()
{
	enabled = true;
}

void ATile::Disable()
{
	enabled = false;
}

bool ATile::IsEnabled()
{
	return enabled;
}

void ATile::BeginPlay()
{
  Super::BeginPlay();
  material = TileMesh->CreateDynamicMaterialInstance(0);
  CancelHighlight();
}

void ATile::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void ATile::Highlight(bool litTile, bool litEdge)
{
	if (litTile)
	{
		HighlightTile();
	}

	if (litEdge)
	{
		HighlightEdge();
	}
}

void ATile::CancelHighlight(bool cancelTile, bool cancelEdge)
{
	if (cancelTile)
	{
		CancelHighlightTile();
	}

	if (cancelEdge)
	{
		CancelHighlightEdge();
	}
}


void ATile::turnOffHighlight(bool offTile, bool offEdge)
{
	if(offTile)
	{
		turnOffHighlightTile();
	}
	if(offEdge)
	{
		turnOffHighlightEdge();
	}
}

void ATile::HighlightTile()
{
	if (material != nullptr)
	{
		material->SetVectorParameterValue(TEXT("BaseColor"), baseColorHighlighted);
	}
}

void ATile::HighlightEdge()
{
	if (material != nullptr)
	{
		material->SetVectorParameterValue(TEXT("Color"), glowColorHighlighted);
	}
}

void ATile::CancelHighlightTile()
{
	if (material != nullptr)
	{
		material->SetVectorParameterValue(TEXT("BaseColor"), baseColor);
	}
}

void ATile::CancelHighlightEdge()
{
	if (material != nullptr)
	{
		material->SetVectorParameterValue(TEXT("Color"), glowColor);
	}
}

void ATile::turnOffHighlightEdge()
{
	if (material != nullptr)
	{
		FLinearColor noColor{ 0.0f, 0.0f, 0.0f };
		material->SetVectorParameterValue(TEXT("Color"), noColor);
	}
}
void ATile::turnOffHighlightTile()
{
	if (material != nullptr)
	{
		FLinearColor noColor{ 0.0f, 0.0f, 0.0f };
		material->SetVectorParameterValue(TEXT("BaseColor"), noColor);
	}
}

void ATile::SetLocationWhenCarried(FVector& location)
{
	SetActorLocation(location);
}
