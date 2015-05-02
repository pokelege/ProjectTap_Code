// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Ground.h"


// Sets default values
AGround::AGround()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));

	this->SetRootComponent(collision);

	collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	collision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
}

// Called when the game starts or when spawned
void AGround::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGround::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

