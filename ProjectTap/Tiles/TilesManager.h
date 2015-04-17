 //Fill out your copyright notice in the Description page of Project Settings.

#pragma once

enum class CurrentTileType : char
{
	NONE,
	BLOCKING_TILE,
	STRONG_BLOCKING_TILE,
	GROUPED_BLOCKING_TILE
};

class UTilesManager
{
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

	UTilesManager();

	void Tick( float DeltaSeconds );

};
