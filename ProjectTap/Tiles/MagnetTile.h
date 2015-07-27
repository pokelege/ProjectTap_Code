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
	AMagnetTile* subMagnet = nullptr;
	static const int32 MAX_DEPTH;
	int32 currentDepth = 0;
	FVector magnetEndPos;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Magnet )
	class UParticleSystemComponent* magnetParticle = nullptr;
	
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
	UAudioComponent* magnetSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnet)
	float length = 5000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnet)
	float targetVelocity = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnet)
	float centerTolerance = 0.001f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnet)
	float verticalForceMultiplier = 1.01f;

	bool isVertical = false;

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
	FVector GetClampedForwardVector(bool infiniteLength = false);

	void UpdateBallPawnSearch(AActor* actor);
	void UpdatePortalSearch(AActor* actor, const FHitResult& hit);

	void SetDepth(int32 depth);

	AActor* GetMagnetHitActor(FHitResult& hit);
	void CastMagnetRay(FHitResult& hit);
	void PullBall( class ABallPawn* ball );
	bool IsMaster();
	void SpawnSubMagnet(const FVector& start, 
						const FVector& dir);
	void KillSubMagnet();

	bool CanSpawnSubMagnet();
};
