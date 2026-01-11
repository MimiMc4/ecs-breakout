#pragma once

#include <ecs/ECS.hpp>

namespace BrickCollideSystem {




	enum Direction {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	void Update(ComponentManager& cm);
}
