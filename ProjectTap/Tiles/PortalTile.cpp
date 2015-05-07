// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "PortalTile.h"

APortalTile::APortalTile()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/Models/Tile"));
	TileMesh->SetStaticMesh(mesh.Object);

	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetBoxExtent(FVector(1.0f));

	bluePortalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BlueTrigger"));
	orangePortalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("OrangeTrigger"));

	bluePortalTrigger->AttachTo(RootComponent);
	bluePortalTrigger->bGenerateOverlapEvents = true;
	
	orangePortalTrigger->AttachTo(RootComponent);
	orangePortalTrigger->bGenerateOverlapEvents = true;

	bluePortalTrigger->SetBoxExtent(FVector(1.0f));
	bluePortalTrigger->SetWorldScale3D(FVector(10.0f, 10.0f, 40.0f));

	orangePortalTrigger->SetBoxExtent(FVector(1.0f));
	orangePortalTrigger->SetWorldScale3D(FVector(10.0f, 10.0f, 40.0f));


	FScriptDelegate beginOverLap;
	beginOverLap.BindUFunction(this, "OnBlueBeginTriggerOverlap_Implementation");
	bluePortalTrigger->OnComponentBeginOverlap.Add(beginOverLap);

	FScriptDelegate endEndLap;
	endEndLap.BindUFunction(this, "OnBlueEndTriggerOverlap_Implementation");
	bluePortalTrigger->OnComponentEndOverlap.Add(endEndLap);

	beginOverLap.BindUFunction(this, "OnOrangeBeginTriggerOverlap_Implementation");
	orangePortalTrigger->OnComponentBeginOverlap.Add(beginOverLap);

	endEndLap.BindUFunction(this, "OnOrangeEndTriggerOverlap_Implementation");
	orangePortalTrigger->OnComponentEndOverlap.Add(endEndLap);

	AdjustOrientation();

}

void APortalTile::AdjustOrientation()
{
	switch (direction)
	{
	case Direction::XPlus:
		RootComponent->SetWorldRotation(FRotator(0, 0, 0));
		bluePortalTrigger->AddLocalOffset(FVector(20.0f, 0.0f, 0.0f));
		orangePortalTrigger->AddLocalOffset(FVector(-20.0f, 0.0f, 0.0f));
		break;
	case Direction::xMinus:
		RootComponent->SetWorldRotation(FRotator(0, 180, 0));
		bluePortalTrigger->AddLocalOffset(FVector(-20.0f, 0.0f, 0.0f));
		orangePortalTrigger->AddLocalOffset(FVector(20.0f, 0.0f, 0.0f));
		break;
	case Direction::YPlus:
		RootComponent->SetWorldRotation(FRotator(0, 90, 0));
		bluePortalTrigger->AddLocalOffset(FVector(.0f, 20.0f, 0.0f));
		orangePortalTrigger->AddLocalOffset(FVector(0.0f, -20.0f, 0.0f));
		break;
	case Direction::yMinus:
		RootComponent->SetWorldRotation(FRotator(0, 270, 0));
		bluePortalTrigger->AddLocalOffset(FVector(0.0f, -20.0f, 0.0f));
		orangePortalTrigger->AddLocalOffset(FVector(0.0f, 20.0f, 0.0f));
		break;
	}
}


void APortalTile::BeginPlay()
{
	Super::BeginPlay();
	AdjustOrientation();
}

void APortalTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APortalTile::OnBlueBeginTriggerOverlap_Implementation(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	bool typeFound = false;
	//auto tile = Cast<ABlockingTileBase>(OtherActor);
	//if (tile != nullptr)
	//{
	//	tile->deactivate();
	//	tile->Disable();
	//	tile->turnOffHighlight();
	//	typeFound = true;
	//}

	//if (!typeFound)
	//{
	//	auto ramp = Cast<ABaseRampTile>(OtherActor);
	//	if (ramp != nullptr)
	//	{
	//		ramp->Enable();
	//		ramp->HighlightTile();
	//		ramp->ball = pawn;
	//		typeFound = true;
	//	}
	//}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor::White, TEXT("blue Overlap"));
	}
}

void APortalTile::OnBlueEndTriggerOverlap_Implementation(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
}

void APortalTile::OnOrangeBeginTriggerOverlap_Implementation(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor::White, TEXT("orange Overlap"));
	}
}

void APortalTile::OnOrangeEndTriggerOverlap_Implementation(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
}