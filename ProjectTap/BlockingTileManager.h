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
	STRONG_BLOCKING_TILE
};

class PROJECTTAP_API BlockingTileManager
{
	std::vector<class ABlockingTile*> activatedBlocks;
	class AStrongBlockingTile* prevStrongBlockingTile;
    unsigned char size_limit{ 3 };
	CurrentTileType currentTileType{CurrentTileType::NONE};

	void SetBlockingTileCurrent();
	void SetStrongBlockingTileCurrent();

	void DeactivateBlockingTiles();
public:
	BlockingTileManager();
	~BlockingTileManager();
	void AddTile(ABlockingTile* tile);
	void AddTile(AStrongBlockingTile* tile);
	void DeactivateStrongBlockingTile();
};
