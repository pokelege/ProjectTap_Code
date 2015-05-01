 //Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "TilesManager.h"
#include "BlockingTile.h"
#include "StrongBlockingTile.h"
#include "GroupedBlockingTile.h"
#include "Ramp.h"

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

	activatedBlocks.RemoveAll([](ABlockingTile* b){return true; });
}

void UTilesManager::DeactivateGroupedBlockingTiles()
{
	for (auto t : activatedGroupedBlocks)
	{
		t->deactivate();
	}

	activatedGroupedBlocks.RemoveAll([](AGroupedBlockingTile* b){return true; });
}

void UTilesManager::HighLightTile(ATile* tile)
{
	if (tile != prevHighlighted && prevHighlighted != nullptr)
	{
		prevHighlighted->CancelHighlight();
	}

	if (tile != nullptr && !tile->isActivated())
	{
		prevHighlighted = tile;
		if (Cast<ARamp>(tile) == nullptr)
		{
			tile->Highlight();
		}
		else
		{
			if (tile->IsEnabled())
			{
				tile->HighlightEdge();
			}
		}
	}
	else if (prevHighlighted != nullptr)
	{
		prevHighlighted->CancelHighlight();
		prevHighlighted = nullptr;
	}
}

void UTilesManager::AddTile(AGroupedBlockingTile* tile)
{

	if (tile != nullptr)
	{
		bool reached_size_limit = activatedGroupedBlocks.Num() >= grouped_size_limit;
		bool hasBeenAdded = false;

		if (activatedGroupedBlocks.Num() > 0)
		{
			for (auto t : activatedGroupedBlocks)
			{
				if (t == tile)
				{
					hasBeenAdded = true;
					break;
				}
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
	if (tile != nullptr && !activatedBlocks.Contains(tile))
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
		auto bt = activatedBlocks[i];
		if (!bt->isActivated())
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


