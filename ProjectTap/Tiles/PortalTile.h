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
	FVector baseColor;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Portal )
		APortalTile* otherPortal = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Portal )
		UBoxComponent* bluePortalTrigger = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Portal )
		UBoxComponent* orangePortalTrigger = nullptr;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
		UAudioComponent* teleportSound = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Portal )
		float velocityMultiplier = 1.0f;
private:
	bool colorSet = false;
	bool leftBluePortal = false;
	bool leftOrangePortal = false;
	bool enteredPortal = false;
public:
	
	APortalTile();

	void SetColor( const FVector& color );
	bool GetColorSet();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	OffsetInfo getOffsetInfo() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;
#endif


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

	void TransportBallToOrange(class ABallPawn* pawn);
	void TransportBallToBlue(ABallPawn* pawn);
	void AdjustOrientationAndTriggerBoxes();

	void SetMeshCollisionProperty( UBoxComponent* box );
	void GeneratePortalCollision();

	friend class ALaser;
	void GetLaserPortalTransportedLocation(UPrimitiveComponent* hit4PportalTrigger, FVector& newDir, FVector& newPos);

	void ProcessBallEndfOverlap(AActor* actor);
};
