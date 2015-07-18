// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CustomGameState.h"
#include "PawnCastingTrigger.generated.h"

UCLASS()
class PROJECTTAP_API APawnCastingTrigger : public AActor
{
	GENERATED_BODY()

	friend class APortalTile;
	friend class ABallPawn;
	FDelegateHandle OnGameStateChangedDelegateHandle;
	class ABallPawn* currentPawn = nullptr;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Ball )
		UBoxComponent* tileOverlapCollision;
private:
	bool canTrigger = true;
public:

	// Sets default values for this actor's properties
	APawnCastingTrigger();
	void BeginPlay() override;

	UFUNCTION()
		void OnBeginTriggerOverlap(AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

	UFUNCTION()
		void OnEndTriggerOverlap(AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);
	UFUNCTION()
	void OnStateChanged( const CustomGameState newState );
private:
	void SetBallPawn( ABallPawn* currentPawn );
};