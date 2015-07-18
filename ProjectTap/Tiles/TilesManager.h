 //Fill out your copyright notice in the Description page of Project Settings.

#pragma once

enum class CurrentTileType : char
{
	NONE,
	BLOCKING_TILE,
	STRONG_BLOCKING_TILE,
	GROUPED_BLOCKING_TILE,
	DRAGGABLE_TILE
};

class UTilesManager
{
	TArray<class ABlockingTile*> activatedBlocks;
	TArray<class AGroupedBlockingTile*> activatedGroupedBlocks;
	FHitResult hit;
	FVector worldOrigin;
	FVector worldDirection;
	class AStrongBlockingTile* prevStrongBlockingTile = nullptr;
	class ATile* prevHighlighted = nullptr;
	class ADraggableMoveTile* currDraggableTile = nullptr;
	class AMouseController* controller = nullptr;
	unsigned char size_limit{ 1 };
	unsigned char grouped_size_limit{ 6 };
	CurrentTileType currentTileType{ CurrentTileType::NONE };
	bool isMousePressed = false;
public:
	UTilesManager();
	void AddTile(ABlockingTile* tile);
	void AddTile(AStrongBlockingTile* tile);
	void AddTile(AGroupedBlockingTile* tile);
	void AddTile(ADraggableMoveTile* tile);
	void DeactivateStrongBlockingTile();
	void SetEnableSwipeCheck(bool b);
	void HighLightTile(ATile* tile);
	void SetCameraRay(const FHitResult& hit,
					  const FVector& worldOrigin,
					  const FVector& worldDirection);
	void MouseRelease();
	void Tick( float DeltaSeconds );
private:
	void UpdateDraggableMoveTile();
	void UpdateGroupedBlockingTiles();
	void SetBlockingTileCurrent();
	void SetStrongBlockingTileCurrent();
	void SetGroupedBlockingTileCurrent();
	void SetDraggableMoveTileCurrent();

	void DeactivateBlockingTiles();
	void DeactivateGroupedBlockingTiles();
};
