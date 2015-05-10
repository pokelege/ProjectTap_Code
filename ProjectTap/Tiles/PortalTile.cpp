// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "PortalTile.h"
#include "../OffensiveTiles/Laser.h"

APortalTile::APortalTile()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/Models/Portal"));
	TileMesh->SetStaticMesh(mesh.Object);

	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);	
	BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetBoxExtent(FVector(1.0f));
	BoxCollision->SetWorldScale3D(FVector(40.0f, 40.0f, 40.0f));

	bluePortalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BlueTrigger"));
	orangePortalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("OrangeTrigger"));

	bluePortalTrigger->AttachTo(RootComponent);
	bluePortalTrigger->bGenerateOverlapEvents = true;
	
	orangePortalTrigger->AttachTo(RootComponent);
	orangePortalTrigger->bGenerateOverlapEvents = true;

	bluePortalTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	orangePortalTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);


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

	SetActorRotation(FRotator(0, 0, 0));

	GeneratePortalCollision();
}

void APortalTile::AdjustOrientation()
{
	bluePortalTrigger->AddLocalOffset(FVector(-.2f, 0.0f, 0.0f));
	orangePortalTrigger->AddLocalOffset(FVector(.2f, 0.0f, 0.0f));
	bluePortalTrigger->SetBoxExtent(FVector(1.0f));
	bluePortalTrigger->SetRelativeScale3D(FVector(.2f, 1.0f, 1.0f));

	orangePortalTrigger->SetBoxExtent(FVector(1.0f));
	orangePortalTrigger->SetRelativeScale3D(FVector(.2f, 1.0f, 1.0f));

	switch (direction)
	{
		case Direction::XPlus:
			SetActorRotation(FRotator(0, 0, 0));
			break;
		case Direction::xMinus:
			SetActorRotation(FRotator(0, 180, 0));
			break;
		case Direction::YPlus:
			SetActorRotation(FRotator(0, 90, 0));
			break;
		case Direction::yMinus:
			SetActorRotation(FRotator(0, 270, 0));
			break;
	}


}

void APortalTile::GeneratePortalCollision()
{
	auto left = CreateDefaultSubobject<UBoxComponent>(TEXT("left"));
	left->AddLocalOffset(FVector(0.0f, 1.0f, 0.0f));
	left->SetRelativeScale3D(FVector(1.0f, 0.1f, 1.0f));

	auto right = CreateDefaultSubobject<UBoxComponent>(TEXT("right"));
	right->AddLocalOffset(FVector(0.0f, -1.0f, 0.0f));
	right->SetRelativeScale3D(FVector(1.0f, 0.1f, 1.0f));

	auto top = CreateDefaultSubobject<UBoxComponent>(TEXT("top"));
	top->AddLocalOffset(FVector(0.0f, 0.0f, 1.f));
	top->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.02f));

	//auto bottom = CreateDefaultSubobject<UBoxComponent>(TEXT("bottom"));
	//bottom->AddLocalOffset(FVector(0.0f, 0.005f, -1.0f));
	//bottom->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.02f));

	SetMeshCollisionProperty(left);
	SetMeshCollisionProperty(right);
	SetMeshCollisionProperty(top);
	//SetMeshCollisionProperty(bottom);

	left->AttachTo(RootComponent);
	right->AttachTo(RootComponent);
	top->AttachTo(RootComponent);
	//bottom->AttachTo(RootComponent);
}

void APortalTile::SetMeshCollisionProperty(UBoxComponent* box)
{
	box->SetBoxExtent(FVector(1.0f));
	box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	box->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
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
	if (enabled)
	{
		if (enteredPortal)
		{
			if (auto a = Cast<ABallPawn>(OtherActor))
			{
				TransportBallToOrange(a);
			}
		}
		else
		{
			enteredPortal = true;
		}
	}
}


void APortalTile::OnOrangeBeginTriggerOverlap_Implementation(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	if (enabled)
	{
		if (enteredPortal)
		{
			if (auto a = Cast<ABallPawn>(OtherActor))
			{
				TransportBallToBlue(a);
			}
		}
		else
		{
			enteredPortal = true;
		}
	}
}

void APortalTile::OnBlueEndTriggerOverlap_Implementation(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	enteredPortal = false;
	leftBluePortal = true;
	Enable();
}


void APortalTile::OnOrangeEndTriggerOverlap_Implementation(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	enteredPortal = false;
	leftOrangePortal = true;
	Enable();
}

void APortalTile::Enable()
{
	if (leftOrangePortal || leftBluePortal)
	{
		enabled = true;
		leftOrangePortal = false;
		leftBluePortal = false;
	}
}

void APortalTile::TransportBallToOrange(ABallPawn* pawn)
{	
	if (otherPortal != nullptr)
	{
		otherPortal->enabled = false;
		auto transportLocation = otherPortal->GetActorLocation();
		transportLocation.Z += 10.0f;
		pawn->SetActorLocation(transportLocation);
		auto newVelMag = pawn->ballCollision->GetPhysicsLinearVelocity().Size();
		auto newVel = newVelMag * otherPortal->GetActorForwardVector();
		pawn->ballCollision->SetPhysicsLinearVelocity(newVel);
		pawn->ballCollision->SetPhysicsAngularVelocity(FVector(0.0f));
	}
}

void APortalTile::TransportBallToBlue(ABallPawn* pawn)
{
	if (otherPortal != nullptr)
	{
		otherPortal->enabled = false;
		auto transportLocation = otherPortal->GetActorLocation();
		transportLocation.Z += 10.0f;
		pawn->SetActorLocation(transportLocation);
		auto newVelMag = pawn->ballCollision->GetPhysicsLinearVelocity().Size();
		auto newVel = newVelMag * -otherPortal->GetActorForwardVector();
		pawn->ballCollision->SetPhysicsLinearVelocity(newVel);
		pawn->ballCollision->SetPhysicsAngularVelocity(FVector(0.0f));
	}
}

void APortalTile::GetLaserPortalTransportedLocation(UPrimitiveComponent* hit4PportalTrigger, FVector& newDir, FVector& newPos)
{
	if (otherPortal != nullptr && hit4PportalTrigger != nullptr)
	{
		bool isPortalTrigger = hit4PportalTrigger == orangePortalTrigger || hit4PportalTrigger == bluePortalTrigger;
		
		if (isPortalTrigger)
		{
			bool isOrangePortal = hit4PportalTrigger == orangePortalTrigger;
			auto newLaserPos = isOrangePortal ? 
				otherPortal->orangePortalTrigger->GetComponentLocation() : 
				otherPortal->bluePortalTrigger->GetComponentLocation();

			newDir = isOrangePortal ? otherPortal->GetActorRotation().Vector() : -otherPortal->GetActorRotation().Vector();
			newPos = newLaserPos + newDir * 10.0f;
		}
	}
}

