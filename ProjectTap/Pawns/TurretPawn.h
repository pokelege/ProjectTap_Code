// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "TurretPawn.generated.h"

UCLASS()
class PROJECTTAP_API ATurretPawn : public APawn
{
	GENERATED_BODY()

	static const FName MESH;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	float FOV = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	float maxDistance = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	float interval = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	USceneComponent* nozzle;

	// Sets default values for this pawn's properties
	ATurretPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	
};
