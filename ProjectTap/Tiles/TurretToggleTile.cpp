// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "TurretToggleTile.h"
#include "Pawns/BallPawn.h"
#include "Pawns/TurretPawn.h"
ATurretToggleTile::ATurretToggleTile():ATile()
{
	PrimaryActorTick.bCanEverTick = false;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*FName("/Game/Models/TurretToggle").ToString());
	TileMesh->SetStaticMesh(mesh.Object);

	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(1,1,1), false);
	}

	if(boxTrigger == nullptr)
	{
		boxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TurretToggleTile box trigger"));
		boxTrigger->AttachTo(this->GetRootComponent());
		boxTrigger->SetBoxExtent(FVector(0.5f,0.5f,2.0f), false);
	}

	boxTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxTrigger->bGenerateOverlapEvents = true;
	boxTrigger->SetNotifyRigidBodyCollision(false);
	boxTrigger->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	boxTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);

	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(40.0f, 40.0f, 80.0f));
	delegate.BindUFunction(this, "OnBeginTriggerOverlap");
	boxTrigger->OnComponentBeginOverlap.Add(delegate);
}

void ATurretToggleTile::OnBeginTriggerOverlap(AActor* OtherActor,
											  UPrimitiveComponent* OtherComp,
											  int32 OtherBodyIndex,
											  bool bFromSweep,
											  const FHitResult & SweepResult)
{
	if(Cast<ABallPawn>(OtherActor) != nullptr && targetTurret != nullptr)
	{
		targetTurret->activated = true;
	}
}