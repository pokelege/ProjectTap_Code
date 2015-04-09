// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Tiles/BlockingTile.h"
#include <vector>
/**
 * 
 */
class PROJECTTAP_API BlockingTileManager
{
  static std::vector<ABlockingTile*> activatedBlocks;
  const static unsigned char size_limit{ 3 };
public:
	BlockingTileManager();
	~BlockingTileManager();
	static void AddTile(ABlockingTile* tile);
};
