 //Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "TilesManager.h"
#include "BlockingTile.h"
#include "StrongBlockingTile.h"
#include "GroupedBlockingTile.h"

 //Sets default values
UTilesManager::UTilesManager()
{
 	 //Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

}



void UTilesManager::DeactivateBlockingTiles()
{
	for (auto t : activatedBlocks)
	{
		t->deactivate();
	}

	activatedBlocks.Empty();
}

void UTilesManager::DeactivateGroupedBlockingTiles()
{
	for (auto t : activatedGroupedBlocks)
	{
		t->deactivate();
	}
	activatedGroupedBlocks.Empty();
}


void UTilesManager::AddTile(AGroupedBlockingTile* tile)
{

	if (tile != nullptr)
	{
		bool reached_size_limit = activatedGroupedBlocks.Num() >= grouped_size_limit;
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
			activatedGroupedBlocks.Add(tile);
			tile->activate();
		}
	}
}

void UTilesManager::AddTile(ABlockingTile* tile)
{
	if (tile != nullptr)
	{
		SetBlockingTileCurrent();

		bool not_full = activatedBlocks.Num() < size_limit;

		if (not_full && !tile->isActivated())
		{
			activatedBlocks.Add(tile);
		}
		else
		{
			DeactivateBlockingTiles();
			activatedBlocks.Add(tile);
		}
		tile->activate();
	}
}

void UTilesManager::AddTile(AStrongBlockingTile* tile)
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

void UTilesManager::Tick(float dt)
{
	UpdateGroupedBlockingTiles();

}

void UTilesManager::UpdateGroupedBlockingTiles()
{
	for (size_t i = 0; i < activatedGroupedBlocks.Num(); i++)
	{
		auto gbt = activatedGroupedBlocks[i];
		if (!gbt->isActivated())
		{
			activatedGroupedBlocks.RemoveAt(i);
			i--;
		}
	}

	for (size_t i = 0; i < activatedBlocks.Num(); i++)
	{
		auto gbt = activatedBlocks[i];
		if (!gbt->isActivated())
		{
			activatedBlocks.RemoveAt(i);
			i--;
		}
	}
}


void UTilesManager::SetEnableSwipeCheck(bool b)
{
	isMousePressed = b;
}

void UTilesManager::DeactivateStrongBlockingTile()
{
	if (prevStrongBlockingTile != nullptr){
		prevStrongBlockingTile->deactivate();
		prevStrongBlockingTile = nullptr;
	}
}

void UTilesManager::SetBlockingTileCurrent()
{
	currentTileType = CurrentTileType::BLOCKING_TILE;
	DeactivateStrongBlockingTile();
	DeactivateGroupedBlockingTiles();
}

void UTilesManager::SetStrongBlockingTileCurrent()
{
	currentTileType = CurrentTileType::STRONG_BLOCKING_TILE;
	DeactivateBlockingTiles();
	DeactivateGroupedBlockingTiles();
}

void UTilesManager::SetGroupedBlockingTileCurrent()
{
	currentTileType = CurrentTileType::GROUPED_BLOCKING_TILE;
	DeactivateBlockingTiles();
	DeactivateStrongBlockingTile();
}

