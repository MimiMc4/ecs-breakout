#pragma once

#include <glm/glm.hpp>

struct Transform {
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	glm::vec2 size = glm::vec2(100.0f, 100.0f);
	float rotate = 0.0f;

};

using Velocity = glm::vec2;