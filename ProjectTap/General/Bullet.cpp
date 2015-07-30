// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Bullet.h"
#include "Pawns/BallPawn.h"
#include "Pawns/TurretPawn.h"

// Sets default values
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;
	ConstructorHelpers::FObjectFinder<UParticleSystem> particle( TEXT( "/Game/Particles/P_Explosion" ) );
	UParticleSystemComponent* Explosion = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "Explosion" ) );
	Explosion->SetTemplate( particle.Object );
	Explosion->bAutoActivate = true;
	particleDoneDelegate.BindUFunction(this , TEXT("OnParticleDone"));
	Explosion->OnSystemFinished.Add( particleDoneDelegate );
	this->SetRootComponent( Explosion );
	ConstructorHelpers::FObjectFinder<USoundBase> explosionSoundFile( TEXT( "/Game/Sound/S_Explosion" ) );
	UAudioComponent* explosionSound = CreateDefaultSubobject<UAudioComponent>( TEXT( "Explosion Sound" ) );
	explosionSound->SetSound( explosionSoundFile.Object );
	explosionSound->bAutoActivate = true;
	soundDoneDelegate.BindUFunction(this , TEXT("OnSoundDone"));
	explosionSound->OnAudioFinished.Add( soundDoneDelegate );
	explosionSound->AttachTo( GetRootComponent() );
}

void ABullet::OnParticleDone( class UParticleSystemComponent* PSystem )
{
	particleDone = true;
	if ( soundDone ) Destroy();
}

void ABullet::OnSoundDone()
{
	soundDone = true;
	if ( particleDone ) Destroy();
}