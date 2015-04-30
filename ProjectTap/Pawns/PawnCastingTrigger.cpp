// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "PawnCastingTrigger.h"
#include "BallPawn.h"
#include "../Tiles/Tile.h"

// Sets default values
APawnCastingTrigger::APawnCastingTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	tileOverlapCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TileOverlap"));
	SetRootComponent(tileOverlapCollision);
	tileOverlapCollision->bGenerateOverlapEvents = true;

	tileOverlapCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	tileOverlapCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	tileOverlapCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	tileOverlapCollision->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
	tileOverlapCollision->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	FScriptDelegate beginOverLap;
	beginOverLap.BindUFunction(this, "OnBeginTriggerOverlap");
	tileOverlapCollision->OnComponentBeginOverlap.Add(beginOverLap);

	FScriptDelegate beginEndLap;
	beginEndLap.BindUFunction(this, "OnEndTriggerOverlap");
	tileOverlapCollision->OnComponentEndOverlap.Add(beginEndLap);


}

void APawnCastingTrigger::SetBallPawn(ABallPawn* pawn)
{
	this->pawn = pawn;
}


// Called when the game starts or when spawned
void APawnCastingTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnCastingTrigger::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (pawn != nullptr)
	{
		tileOverlapCollision->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		tileOverlapCollision->SetRelativeScale3D(FVector(10.2f, 10.2f, 10.2f));
	}

}

void APawnCastingTrigger::OnBeginTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	auto tile = Cast<ATile>(OtherActor);
	if (tile != nullptr)
	{
		tile->deactivate();
		tile->Disable();
		tile->turnOffHighlight();
	}
}

void APawnCastingTrigger::OnEndTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	auto tile = Cast<ATile>(OtherActor);
	if (tile != nullptr)
	{
		tile->Enable();
		tile->CancelHighlight();
	}
}