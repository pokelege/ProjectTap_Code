// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
/**
 * 
 */
enum class CurrentTileType : char
{
	NONE,
	BLOCKING_TILE,
	STRONG_BLOCKING_TILE,
	GROUPED_BLOCKING_TILE
};

class PROJECTTAP_API BlockingTileManager
{
	std::vector<class ABlockingTile*> activatedBlocks;
	std::vector<class AGroupedBlockingTile*> activatedGroupedBlocks;
	class AStrongBlockingTile* prevStrongBlockingTile;
	bool isMousePressed = false;

	unsigned char size_limit{ 3 };
	unsigned char grouped_size_limit{ 6 };
	CurrentTileType currentTileType{CurrentTileType::NONE};

	void UpdateGroupedBlockingTiles();

	void SetBlockingTileCurrent();
	void SetStrongBlockingTileCurrent();
	void SetGroupedBlockingTileCurrent();

	void DeactivateBlockingTiles();
	void DeactivateGroupedBlockingTiles();
public:
	BlockingTileManager();
	~BlockingTileManager();
	void AddTile(ABlockingTile* tile);
	void AddTile(AStrongBlockingTile* tile);
	void AddTile(AGroupedBlockingTile* tile);
	void DeactivateStrongBlockingTile();
	void Tick(float dt);
	void SetEnableSwipeCheck(bool b);
};
