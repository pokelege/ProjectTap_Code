 //Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TilesManager.generated.h"

enum class CurrentTileType : char
{
	NONE,
	BLOCKING_TILE,
	STRONG_BLOCKING_TILE,
	GROUPED_BLOCKING_TILE
};

UCLASS()
class PROJECTTAP_API UTilesManager : public UObject
{
	GENERATED_BODY()

	TArray<class ABlockingTile*> activatedBlocks;
	TArray<class AGroupedBlockingTile*> activatedGroupedBlocks;
	class AStrongBlockingTile* prevStrongBlockingTile;
	bool isMousePressed = false;

	unsigned char size_limit{ 3 };
	unsigned char grouped_size_limit{ 6 };
	CurrentTileType currentTileType{ CurrentTileType::NONE };

	void UpdateGroupedBlockingTiles();

	void SetBlockingTileCurrent();
	void SetStrongBlockingTileCurrent();
	void SetGroupedBlockingTileCurrent();

	void DeactivateBlockingTiles();
	void DeactivateGroupedBlockingTiles();
public:
	
	void AddTile(ABlockingTile* tile);
	void AddTile(AStrongBlockingTile* tile);
	void AddTile(AGroupedBlockingTile* tile);
	void DeactivateStrongBlockingTile();
	void SetEnableSwipeCheck(bool b);

	 //Sets default values for this actor's properties
	UTilesManager();

	 //Called every frame
	void Tick( float DeltaSeconds );

	
	
};
