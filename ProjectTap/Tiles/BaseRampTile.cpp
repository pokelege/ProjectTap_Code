// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BaseRampTile.h"

const FName ABaseRampTile::BASE_RAMP_CURVE_PATH = FName("/Game/Curves/Ramp");

ABaseRampTile::ABaseRampTile() : ATile()
{
	if(BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(1,1,1), false);
		BoxCollision->SetRelativeLocation(FVector(0, 0, -10), false, nullptr);
		BoxCollision->AddLocalOffset(FVector(0, 0, -10));
	}

	if(boxTrigger == nullptr)
	{
		boxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Ramp box trigger"));
		boxTrigger->AttachTo(this->GetRootComponent());
		boxTrigger->SetBoxExtent(FVector(0.5f,0.5f,2.0f), false);
	}

	boxTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxTrigger->bGenerateOverlapEvents = true;
	boxTrigger->SetNotifyRigidBodyCollision(false);
	boxTrigger->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	boxTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);

	pawnIn.BindUFunction(this, "OnBeginTriggerOverlap");
	boxTrigger->OnComponentBeginOverlap.Add(pawnIn);
	pawnOut.BindUFunction(this, "OnEndTriggerOverlap");
	boxTrigger->OnComponentEndOverlap.Add(pawnOut);
	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(*BASE_RAMP_CURVE_PATH.ToString());
	if(curve.Object != nullptr) rotationSequence = curve.Object;

	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(40.0f, 40.0f, 2.0f));

	baseColorHighlighted = FLinearColor(0.8f, 0.0f, .3f);
	glowColorHighlighted = FLinearColor(2.0f, 1.7f, .3f);
	baseColor = FLinearColor(0.2f, 0.5f, .3f);
	glowColor = FLinearColor(1.0f, .7f, .0f);
	glowPowerHighlighted = 100.0f;
	CancelHighlight();
}

void ABaseRampTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(activated)
	{
		float minTime, maxTime;
		rotationSequence->GetTimeRange(minTime, maxTime);
		if((time += DeltaTime) >= maxTime)
		{
			time = maxTime;
			deactivate();
		}
		float curveValue = rotationSequence->GetFloatValue(time);
		TileMesh->SetRelativeRotation(FRotator(curveValue, 0, 0));
	}

}

void ABaseRampTile::activate()
{
	if(rotationSequence == nullptr || ball == nullptr || activated) return;
	Super::activate();
	time = 0;
	auto xyPositionAdjustment = GetActorLocation();
	ball->ResetBallXYPosition(GetActorLocation());
}

void ABaseRampTile::Highlight(bool litTile, bool litEdge)
{
	Super::Highlight(false, true);
}

void ABaseRampTile::OnBeginTriggerOverlap(AActor* OtherActor,
								  UPrimitiveComponent* OtherComp,
								  int32 OtherBodyIndex,
								  bool bFromSweep,
								  const FHitResult & SweepResult)
{
	ball = Cast<ABallPawn>(OtherActor);
	Super::Highlight(true, false);
}

void ABaseRampTile::OnEndTriggerOverlap(AActor* OtherActor,
								UPrimitiveComponent* OtherComp,
								int32 OtherBodyIndex)
{
	if(Cast<ABallPawn>(OtherActor) != nullptr) ball = nullptr;
	CancelHighlight();
}