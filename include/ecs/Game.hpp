#pragma once

#include <ecs/EntityComponent.hpp>

#include <SDL3/SDL.h>

#include <map>

enum class GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_END
};


struct RawInput {
	std::map<SDL_Keycode, bool>  Keys;
};





void ECS_InitGameEntity();


