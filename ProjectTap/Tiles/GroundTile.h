// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GroundTile.generated.h"

UCLASS()
class PROJECTTAP_API AGroundTile : public AActor
{
	GENERATED_BODY()
private:
public:
	UPROPERTY()
		FVector location = FVector( 0 );
	UPROPERTY()
		UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Attachment )
		UClass* ActorToCreate = nullptr;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Attachment )
		AActor* ActorToAttach = nullptr;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Attachment )
		bool DestroyActorWithGround = true;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Attachment )
		bool GenerateActorButton = false;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Mesh )
		bool GroundVisible = true;
	// Sets default values for this actor's properties
	AGroundTile();
	void UpdateAttachedLocation();
	void AttachActor();
#if WITH_EDITOR
	virtual void PostEditChangeProperty( FPropertyChangedEvent & PropertyChangedEvent ) override;
#endif
};
