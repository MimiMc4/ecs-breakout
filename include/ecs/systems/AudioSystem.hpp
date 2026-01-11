#pragma once

#include <ecs/ECS.hpp>

#include <ecs/components/AudioComponents.hpp>

#include <ecs/EntityComponent.hpp>
#include <render/ResourceManager.hpp>

#include <miniaudio.h>


namespace AudioSystem {
	void Update(ComponentManager& cm);
}