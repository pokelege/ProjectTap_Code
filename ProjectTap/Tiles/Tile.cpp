// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Tile.h"
#include "EmptyComponent.h"
// Sets default values
ATile::ATile()
{
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Tile mesh" ) );

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Tile collision"));

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
	activated = true;
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
  original = GetActorLocation();
}

void ATile::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void ATile::Highlight(bool litTile, bool litEdge)
{
	if (!activated)
	{
		material = TileMesh->CreateAndSetMaterialInstanceDynamic(0);

		if (material != nullptr)
		{
			if (litTile)
			{
				material->SetVectorParameterValue(TEXT("BaseColor"), baseColorHighlighted);
			}

			if (litEdge)
			{
				material->SetScalarParameterValue(TEXT("glow"), glowPowerHighlighted);
				material->SetVectorParameterValue(TEXT("Color"), glowColorHighlighted);
			}
		}
	}
}

void ATile::CancelHighlight()
{
	material = TileMesh->CreateAndSetMaterialInstanceDynamic(0);

	if (material != nullptr)
	{
		material->SetVectorParameterValue(TEXT("BaseColor"), baseColor);
		material->SetScalarParameterValue(TEXT("glow"), glowPower);
		material->SetVectorParameterValue(TEXT("Color"), glowColor);
	}
}


