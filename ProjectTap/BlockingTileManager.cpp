// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Tiles/StrongBlockingTile.h"
#include "Tiles/BlockingTile.h"
#include "Tiles/GroupedBlockingTile.h"
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

void BlockingTileManager::DeactivateGroupedBlockingTiles()
{
	for (auto t : activatedGroupedBlocks)
	{
		t->deactivate();
	}
	activatedGroupedBlocks.clear();
}


void BlockingTileManager::AddTile(AGroupedBlockingTile* tile)
{
	
	if (tile != nullptr )
	{	
		bool reached_size_limit = activatedGroupedBlocks.size() >= grouped_size_limit;
		bool hasBeenAdded = false;

		for (auto t : activatedGroupedBlocks)
		{
			if (t == tile)
			{
				hasBeenAdded = true;
				break;
			}
		}

		if (!reached_size_limit && !hasBeenAdded)
		{
			SetGroupedBlockingTileCurrent();
			activatedGroupedBlocks.push_back(tile);
			tile->activate();
		}
	}
}

void BlockingTileManager::AddTile(ABlockingTile* tile)
{
	if (tile != nullptr)
	{
		SetBlockingTileCurrent();

		bool not_full = activatedBlocks.size() < size_limit;

		if (not_full && !tile->isActivated())
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

void BlockingTileManager::Tick(float dt)
{
	UpdateGroupedBlockingTiles();

}

void BlockingTileManager::UpdateGroupedBlockingTiles()
{
	for (size_t i = 0; i < activatedGroupedBlocks.size(); i++)
	{
		auto gbt = activatedGroupedBlocks[i];
		if (!gbt->isActivated())
		{
			activatedGroupedBlocks.erase(activatedGroupedBlocks.begin() + i);
			i--;
		}
	}

}


void BlockingTileManager::SetEnableSwipeCheck(bool b)
{
	isMousePressed = b;
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
	DeactivateGroupedBlockingTiles();
}

void BlockingTileManager::SetStrongBlockingTileCurrent()
{
	currentTileType = CurrentTileType::STRONG_BLOCKING_TILE;
	DeactivateBlockingTiles();
	DeactivateGroupedBlockingTiles();
}

void BlockingTileManager::SetGroupedBlockingTileCurrent()
{
	currentTileType = CurrentTileType::GROUPED_BLOCKING_TILE;
	DeactivateBlockingTiles();
	DeactivateStrongBlockingTile();
}
