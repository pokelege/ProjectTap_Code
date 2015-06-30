// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class PROJECTTAP_API ABullet : public AActor
{
	GENERATED_BODY()

	static const FName MESH;
	TScriptDelegate<FWeakObjectPtr> delegate;
public:
	// Sets default values for this actor's properties
	ABullet();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
	void OnBeginHit(class AActor* OtherActor,
					class UPrimitiveComponent* OtherComp,
					FVector NormalImpulse,
					const FHitResult& Hit);
};
