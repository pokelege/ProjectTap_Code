// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "IGroundable.h"
#include "CustomGameState.h"
#include "Tile.generated.h"

UCLASS()
/// <summary>
/// The base tile
/// </summary>
class PROJECTTAP_API ATile : public AActor, public IGroundable
{
	GENERATED_BODY()
private:
	static const GroundableInfo groundableInfo;
protected:
	FDelegateHandle OnGameStateChangedDelegateHandle;
	UMaterialInstanceDynamic* material = nullptr;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		UStaticMeshComponent* TileMesh = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Tile )
		UBoxComponent* BoxCollision = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Color )
		FLinearColor baseColor = FLinearColor( 1.0f , 0.0f , 0.0f );

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Color )
		FLinearColor glowColor = FLinearColor( 1.0 , 0.537034 , 0.077178 );

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Color )
		FLinearColor baseColorHighlighted = FLinearColor( 5.f , 0.0f , .0f );

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Color )
		FLinearColor glowColorHighlighted = FLinearColor( 5.0f , 5.0f , 5.0f );

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Color )
		float glowPowerHighlighted = 20.0f;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Color )
		float glowPower = 10.0f;
protected:
	bool activated = false;
	bool enabled = true;
	bool canActivate = true;
public:
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION( BlueprintCallable , Category = "Tile" )
		virtual void activate();

	UFUNCTION( BlueprintCallable , Category = "Tile" )
		virtual void deactivate();
	UFUNCTION()
	virtual void OnGameStateChanged( const CustomGameState gameState );
	virtual bool isActivated();

	virtual void Enable();
	virtual void Disable();
	virtual bool IsEnabled();

	virtual void BeginPlay();
	virtual void Tick( float DeltaTime );

	virtual void Highlight( bool litTile = true , bool litEdge = true );
	virtual void HighlightTile();
	virtual void HighlightEdge();
	virtual void CancelHighlightTile();
	virtual void CancelHighlightEdge();
	virtual void turnOffHighlightEdge();
	virtual void turnOffHighlightTile();

	virtual void CancelHighlight( bool cancelTile = true , bool cancelEdge = true );
	virtual void turnOffHighlight( bool offTile = true , bool offEdge = true );

	//called when the tile is on the moving tile
	virtual void SetLocationWhenCarried( FVector& location );
	virtual const struct GroundableInfo* GetGroundableInfo() const override;
};
