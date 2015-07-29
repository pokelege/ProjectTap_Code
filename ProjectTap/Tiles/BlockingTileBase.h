// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "BlockingTileBase.generated.h"

UCLASS()
class PROJECTTAP_API ABlockingTileBase : public ATile
{
	GENERATED_BODY()
protected:
	USoundBase* ActivateSound = nullptr;
	USoundBase* DeactivateSound = nullptr;
	UAudioComponent* SoundPlayer = nullptr;
	FVector original;
	float time_counter = 0.0f;

public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		float move_speed = 800.0f;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		float move_distance_tolerance = 100.0f;

	// Sets default values for this actor's properties
	ABlockingTileBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds )override;

	bool CanKillBall();

	bool CanStopBall();

	void deactivate() override;
	void activate() override;




protected:
	virtual void lerpMaterialColorForCoolDown( const float& beta );
};
