// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GroundTileManager.h"
#include "GroundTile.h"
// Sets default values
AGroundTileManager::AGroundTileManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Collision->SetMobility(EComponentMobility::Static);
}

void AGroundTileManager::Destroyed()
{
	if (DestroyActorsWithGroundManager)
	{
		for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (ActorItr->IsAttachedTo(this)) ActorItr->Destroy();
		}
	}
	Super::Destroyed();
}

void AGroundTileManager::Generate()
{
	FVector boxExtent((float)NumTilesX * MeshScaleX, (float)NumTilesY * MeshScaleY, MeshScaleZ);
	Collision->SetBoxExtent(boxExtent);
	TMap<FVector, AGroundTile*> GroundTiles;

	for (TActorIterator<AGroundTile> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsAttachedTo(this))
		{
			if (ActorItr->location.Equals(FVector::ZeroVector) || GroundTiles.Contains(ActorItr->location))
			{
				ActorItr->Destroy();
			}
			else
			{
				GroundTiles.Add(ActorItr->location, *ActorItr);
			}
		}
	}

	for (int x = 1; x <= NumTilesX; ++x)
	{
		for (int y = 1; y <= NumTilesY; ++y)
		{
			AGroundTile* newGround = nullptr;
			FVector currentLocation(x, y, 0);
			if (GroundTiles.Contains(currentLocation))
			{
				newGround = *GroundTiles.Find(currentLocation);
				newGround->AttachRootComponentToActor(this);
				newGround->UpdateAttachedLocation();
				GroundTiles.Remove(currentLocation);
			}
			else
			{
				newGround = GetWorld()->SpawnActor<AGroundTile>();
				newGround->location = currentLocation;
				newGround->AttachRootComponentToActor(this);
			}
			FVector translation(0);
			//todo optimize if possible
			//translation.X = ( ( x - 1 ) * ( MeshScaleX * 2.0f ) ) - ( ( ( NumTilesX - 1 ) * ( MeshScaleX * 2.0f ) ) / 2.0f );
			translation.X = MeshScaleX * ((2.0f * x) - NumTilesX - 1);
			//translation.Y = ( ( y - 1 ) * ( MeshScaleY * 2.0f ) ) - ( ( ( NumTilesY - 1 ) * ( MeshScaleY * 2.0f ) ) / 2.0f );
			translation.Y = MeshScaleY * ((2.0f * y) - NumTilesY - 1);
			newGround->SetActorRelativeLocation(translation);
			newGround->SetActorRelativeRotation(FRotator(0));
			newGround->SetActorRelativeScale3D(FVector(1));
		}
	}
	for (auto toDelete = GroundTiles.CreateIterator(); toDelete; ++toDelete)
	{
		toDelete->Value->Destroy();
	}
}

#if WITH_EDITOR
void AGroundTileManager::EditorKeyPressed(FKey Key,
	EInputEvent Event)
{
	Super::EditorKeyPressed(Key, Event);
	if (!IsSelected()) return;
	if (Event == EInputEvent::IE_Released && Key == EKeys::Enter)
	{
		Generate();
	}
}

void AGroundTileManager::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr && (PropertyChangedEvent.Property->GetNameCPP().Equals("ApplyProperties_Button") ||
		PropertyChangedEvent.Property->GetNameCPP().Equals("NumTilesX") ||
		PropertyChangedEvent.Property->GetNameCPP().Equals("NumTilesY")))
		Generate();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
