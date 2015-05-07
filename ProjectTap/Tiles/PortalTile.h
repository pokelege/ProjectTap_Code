// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Ramp.h"
#include "PortalTile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTAP_API APortalTile : public ATile
{
	GENERATED_BODY()

		void AdjustOrientation();
public:
	/*blue portal faces positive X axis*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Portal)
		Direction direction = Direction::XPlus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Portal)
	APortalTile* otherPortal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Portal)
	UBoxComponent* bluePortalTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Portal)
	UBoxComponent* orangePortalTrigger;

	APortalTile();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void OnBlueBeginTriggerOverlap_Implementation(AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

	UFUNCTION()
		virtual void OnBlueEndTriggerOverlap_Implementation(AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

	UFUNCTION()
		virtual void OnOrangeBeginTriggerOverlap_Implementation(AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

	UFUNCTION()
		virtual void OnOrangeEndTriggerOverlap_Implementation(AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

private:
	bool enteredFromBluePortal = false;

};
