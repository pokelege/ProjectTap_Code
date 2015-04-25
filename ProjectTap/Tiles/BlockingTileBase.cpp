// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BlockingTileBase.h"


// Sets default values
ABlockingTileBase::ABlockingTileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision->SetBoxExtent(FVector(1.0f, 1.0f, 1.0f));

	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ABlockingTileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlockingTileBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	auto pos = GetActorLocation();
	bool canRise = pos.Z - original.Z < move_distance_tolerance;
	bool canDesend = pos.Z - original.Z > FLT_EPSILON;

	if (activated && canRise)
	{
		pos.Z += move_speed * DeltaTime;
	}
	else if (activated && !canRise)
	{
		pos.Z = original.Z + move_distance_tolerance;
	}
	else if (!activated && canDesend)
	{
		pos.Z -= move_speed * DeltaTime;
	}
	else if (!activated)
	{
		pos.Z = original.Z;
	}

	SetActorLocation(pos);

}

void ABlockingTileBase::lerpMaterialColorForCoolDown(const float& beta)
{
	material = TileMesh->CreateAndSetMaterialInstanceDynamic(0);

	if (material != nullptr)
	{
		auto lerp_baseColor = FMath::Lerp(baseColorHighlighted, baseColor, beta);
		material->SetVectorParameterValue(TEXT("BaseColor"), lerp_baseColor);

		auto lerp_glowPower = FMath::Lerp(glowPowerHighlighted, glowPower, beta);
		material->SetScalarParameterValue(TEXT("glow"), lerp_glowPower);

		auto lerp_glowColor= FMath::Lerp(glowColorHighlighted, glowColor, beta);
		material->SetVectorParameterValue(TEXT("Color"), lerp_glowColor);
	}
}
