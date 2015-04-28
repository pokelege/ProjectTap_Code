// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "TurretToggleTile.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTTAP_API ATurretToggleTile : public ATile
{
	GENERATED_BODY()
	
	TScriptDelegate<FWeakObjectPtr> delegate;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	class ATurretPawn* targetTurret;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	UBoxComponent* boxTrigger;

    ATurretToggleTile();

	UFUNCTION()
	void OnBeginTriggerOverlap(AActor* OtherActor,
							   UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex,
							bool bFromSweep,
							const FHitResult & SweepResult);
};
