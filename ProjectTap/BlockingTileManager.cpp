// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Tiles/StrongBlockingTile.h"
#include "Tiles/BlockingTile.h"
#include "BlockingTileManager.h"

BlockingTileManager::BlockingTileManager()
{
}

BlockingTileManager::~BlockingTileManager()
{
	activatedBlocks.clear();
}

void BlockingTileManager::DeactivateBlockingTiles()
{
	for (auto t : activatedBlocks)
	{
		t->deactivate();
	}

	activatedBlocks.clear();
}


void BlockingTileManager::AddTile(ABlockingTile* tile)
{
	if (tile != nullptr)
	{	
		SetBlockingTileCurrent();

		bool not_full = activatedBlocks.size() < size_limit;

		if (not_full)
		{
			activatedBlocks.push_back(tile);
		}
		else
		{
			DeactivateBlockingTiles();
			activatedBlocks.push_back(tile);
		}
		tile->activate();
	}
}

void BlockingTileManager::AddTile(AStrongBlockingTile* tile)
{	
	if (tile != nullptr && tile != prevStrongBlockingTile)
	{
		SetStrongBlockingTileCurrent();

		if (prevStrongBlockingTile != nullptr){
			prevStrongBlockingTile->deactivate();
		}
		prevStrongBlockingTile = tile;
		tile->activate();
	}
}

void BlockingTileManager::DeactivateStrongBlockingTile()
{
	if (prevStrongBlockingTile != nullptr){
		prevStrongBlockingTile->deactivate();
		prevStrongBlockingTile = nullptr;
	}
}

void BlockingTileManager::SetBlockingTileCurrent()
{
	currentTileType = CurrentTileType::BLOCKING_TILE;
	DeactivateStrongBlockingTile();
}

void BlockingTileManager::SetStrongBlockingTileCurrent()
{
	currentTileType = CurrentTileType::STRONG_BLOCKING_TILE;
	DeactivateBlockingTiles();
}