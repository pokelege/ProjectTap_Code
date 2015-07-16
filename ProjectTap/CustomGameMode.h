#pragma once
#include "ObjectBase.h"
UENUM( CustomGameMode )
enum class CustomGameMode : uint8
{
	GAME_MODE_UNKNOWN = 0 ,
	GAME_MODE_LEVEL = 1 ,
	GAME_MODE_MAIN_MENU = 2
};