#pragma once

#include <SDL3/SDL.h>

struct Key {
	SDL_Keycode keycode;
	bool active;
};

struct Input {
	Key left = { SDLK_LEFT, false };
	Key right = { SDLK_RIGHT, false };
	Key space = { SDLK_SPACE, false };
};