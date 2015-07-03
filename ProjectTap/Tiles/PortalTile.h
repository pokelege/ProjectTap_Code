// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "ICarriable.h"
#include "PortalTile.generated.h"

/**
 * blue portal faces positive X axis
 */
UCLASS()
class PROJECTTAP_API APortalTile : public ATile, public ICarriable
{
	GENERATED_BODY()

	TScriptDelegate<FWeakObjectPtr> beginBlueOverlap;
	TScriptDelegate<FWeakObjectPtr> endBlueOverlap;
	TScriptDelegate<FWeakObjectPtr> beginOrangeOverlap;
	TScriptDelegate<FWeakObjectPtr> endOrangeOverlap;
	void AdjustOrientationAndTriggerBoxes();

	void SetMeshCollisionProperty(UBoxComponent* box);
	void GeneratePortalCollision();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Portal)
	float velocityMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Portal)
	APortalTile* otherPortal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Portal)
	UBoxComponent* bluePortalTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Portal)
	UBoxComponent* orangePortalTrigger;

	APortalTile();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	OffsetInfo getOffsetInfo() override;

	UFUNCTION()
	void OnBlueBeginTriggerOverlap(class AActor* OtherActor,
									class UPrimitiveComponent* OtherComp,
									int32 OtherBodyIndex,
									bool bFromSweep,
									const FHitResult & SweepResult);

	UFUNCTION()
	void OnBlueEndTriggerOverlap(class AActor* OtherActor,
										 class UPrimitiveComponent* OtherComp,
										 int32 OtherBodyIndex,
										 bool bFromSweep,
										 const FHitResult & SweepResult);


	UFUNCTION()
	void OnOrangeBeginTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnOrangeEndTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult);

	void Enable() override;

private:
	bool leftBluePortal = false;
	bool leftOrangePortal = false;
	bool enteredPortal = false;

	void TransportBallToOrange(class ABallPawn* pawn);
	void TransportBallToBlue(ABallPawn* pawn);

	friend class ALaser;
	void GetLaserPortalTransportedLocation(UPrimitiveComponent* hit4PportalTrigger, FVector& newDir, FVector& newPos);

	void ProcessBallEndfOverlap(AActor* actor);
};
