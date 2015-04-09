// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BlockingTileManager.h"

std::vector<ABlockingTile*> BlockingTileManager::activatedBlocks = std::vector<ABlockingTile*>();

BlockingTileManager::BlockingTileManager()
{
}

BlockingTileManager::~BlockingTileManager()
{
  
}

void BlockingTileManager::AddTile(ABlockingTile* tile)
{
	bool not_full = activatedBlocks.size() < size_limit;

	if (not_full)
	{
		BlockingTileManager::activatedBlocks.push_back(tile);
	}
	else
	{
		for (auto t : activatedBlocks)
		{
			t->deactivate();
		}

		BlockingTileManager::activatedBlocks.clear();
		BlockingTileManager::activatedBlocks.push_back(tile);
	}
	tile->activate();
}