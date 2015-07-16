#pragma once
#include "ObjectBase.h"
UENUM( CustomGameState )
enum class CustomGameState : uint8
{
	GAME_STATE_UNKNOWN = 0 ,
	GAME_STATE_PLAYING = 1 ,
	GAME_STATE_DYING = 2 ,
	GAME_STATE_WINNING = 3,
	GAME_STATE_GAME_OVER = 4 ,
	GAME_STATE_WIN = 5,
	GAME_STATE_MAIN_MENU = 6,
	GAME_STATE_PAUSE = 7
};