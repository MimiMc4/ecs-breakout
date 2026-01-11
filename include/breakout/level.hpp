#pragma once

#include <ecs/ECS.hpp>

constexpr unsigned int MAX_BRICKS = 200;

struct BrickArray {
	std::array<Entity, MAX_BRICKS> Bricks;
	unsigned int count = 0;
};

struct Brick {
	bool isSolid = false;
	bool destroyed = false;
};


namespace breakout{

void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);

void Init(std::array<unsigned int, MAX_BRICKS> tileData, unsigned int tileCount, unsigned int lineSize, unsigned int levelWidth, unsigned int levelHeight);

}