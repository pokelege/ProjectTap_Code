// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PlayerBall.generated.h"

UCLASS()
class PROJECTTAP_API APlayerBall : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
		UStaticMeshComponent* ballMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
		USphereComponent* ballCollision;

	// Sets default values for this actor's properties
	APlayerBall();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "Ball")
		void AddVelocity(const FVector& vel);
	
};
