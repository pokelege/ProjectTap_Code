// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Laser.generated.h"

UCLASS()
class PROJECTTAP_API ALaser : public AActor
{
	GENERATED_BODY()
		float timer = .0f;
		float elapseTime = 0.1f;
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Laser)
	UParticleSystemComponent* laserParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Laser)
	FVector direction = FVector(1.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Laser)
	float length = 1000.0f;

	// Sets default values for this actor's properties
	ALaser();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
