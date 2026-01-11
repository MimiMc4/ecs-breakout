#pragma once

#include <ecs/ECS.hpp>

#include <algorithm>

namespace PlayerLogicSystem {
	void Update(ComponentManager& cm, uint64_t deltaTime);
}