#pragma once

#include <ecs/ECS.hpp>

#include <ecs/components/PhysicsComponents.hpp>

#include <ecs/EntityComponent.hpp>


namespace PhysicsSystem {
	void Update(ComponentManager& cm, uint64_t deltaTime);
}