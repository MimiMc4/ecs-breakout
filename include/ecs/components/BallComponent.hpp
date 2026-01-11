#pragma once

#include <ecs/entityComponent.hpp>
#include <glm/glm.hpp>

// Initial velocity of the Ball
constexpr glm::vec2 INITIAL_BALL_VELOCITY = glm::vec2(300.0f, -500.0f);
constexpr float MAX_BALL_VELOCITY_X = 500.0f;
constexpr float MAX_BALL_VELOCITY_Y = 500.0f;

struct Ball {
	bool stuck = false;
	Entity parent = NULL_INDEX;
};