// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PawnCastingTrigger.generated.h"

UCLASS()
class PROJECTTAP_API APawnCastingTrigger : public AActor
{
	GENERATED_BODY()

	class ABallPawn* pawn = nullptr;

private:
	friend ABallPawn;
	void SetBallPawn(ABallPawn* pawn);
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
		UBoxComponent* tileOverlapCollision;

	// Sets default values for this actor's properties
	APawnCastingTrigger();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

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
	
};
