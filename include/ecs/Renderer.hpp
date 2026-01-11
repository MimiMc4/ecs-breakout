#pragma once

#include <SDL3/SDL.h>

struct WindowInfo{
	unsigned int Width = 0;
	unsigned int Height = 0;
	SDL_Window* window = NULL;
};


void ECS_InitRendererEntity(WindowInfo windowInfoComponent);

// Hay que añadirle un shader por defecto más adelante

