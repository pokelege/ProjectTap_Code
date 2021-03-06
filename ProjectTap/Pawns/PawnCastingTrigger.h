// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CustomGameState.h"
#include "PawnCastingTrigger.generated.h"

enum class BallCastType : uint8
{
	CAST_RAMP = 0,
	CAST_BLOCKING = 1
};

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
	friend class ABallPawn;
	bool isOverlaping = false;
public:
	BallCastType castType = BallCastType::CAST_RAMP;

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