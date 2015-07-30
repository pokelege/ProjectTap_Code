// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class PROJECTTAP_API ABullet : public AActor
{
	GENERATED_BODY()

	TScriptDelegate<FWeakObjectPtr> particleDoneDelegate;
	TScriptDelegate<FWeakObjectPtr> soundDoneDelegate;
	bool particleDone = false;
	bool soundDone = false;
public:
	// Sets default values for this actor's properties
	ABullet();
	UFUNCTION()
	void OnParticleDone( class UParticleSystemComponent* PSystem );
	UFUNCTION()
	void OnSoundDone();
};
