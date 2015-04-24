// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Laser.h"

// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	laserParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserParticle"));

	SetRootComponent(laserParticle);

	ConstructorHelpers::FObjectFinder<UParticleSystem> particleAssets(TEXT("/Game/Particles/P_Laser"));
	laserParticle->SetTemplate(particleAssets.Object);
		
	laserParticle->SetWorldLocation(FVector(0.0f));
	
	SetActorLocation(FVector(0.0f, 100.0f, 0.0f));
}

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALaser::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (timer <= elapseTime)
	{
		timer += DeltaTime;
	}
	else
	{
		FHitResult hit;
		FCollisionQueryParams queryParam;
		FCollisionObjectQueryParams objectParam ;
		GetWorld()->LineTraceSingle(hit, GetActorLocation(), GetActorForwardVector() * length, queryParam, objectParam);

		if (hit.Actor.Get() != nullptr)
		{
			laserParticle->SetBeamTargetPoint(0, hit.ImpactPoint, 0);

			auto p = Cast<ALaser>(hit.Actor.Get());
			auto l = Cast<UParticleSystemComponent>(hit.Actor.Get());
			int i = 0;
		}
	}

	laserParticle->SetBeamEndPoint(0, GetActorForwardVector() + direction * length);
}

