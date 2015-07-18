// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "ICarriable.h"
#include "MagnetTile.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API AMagnetTile : public ATile, public ICarriable
{
	GENERATED_BODY()

	TScriptDelegate<FWeakObjectPtr> delegate;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Magnet )
	class UParticleSystemComponent* magnetParticle = nullptr;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Magnet )
		UStaticMeshComponent* distortionMesh = nullptr;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
		UAudioComponent* magnetSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnet)
	float length = 99999.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnet)
	float targetVelocity = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnet)
	float centerTolerance = 0.001f;
	AMagnetTile();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;
	OffsetInfo getOffsetInfo() override;

	UFUNCTION()
		void OnBeginHit( class AActor* OtherActor ,
	class UPrimitiveComponent* OtherComp ,
		FVector NormalImpulse ,
		const FHitResult& Hit );

	virtual void deactivate() override;
	virtual void activate() override;
private:
	class ABallPawn* FindBallPawn();
	void PullBall( class ABallPawn* ball , float DeltaTime );
};
