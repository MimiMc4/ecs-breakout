#pragma once

#include <ecs/EntityComponent.hpp>
#include <ecs/Game.hpp>
#include <ecs/Renderer.hpp>

#include <ecs/systems/RenderSystem.hpp>
#include <ecs/systems/AudioSystem.hpp>
#include <ecs/systems/PhysicsSystem.hpp>
#include <ecs/systems/InputSystem.hpp>
#include <ecs/systems/PlayerLogicSystem.hpp>
#include <ecs/systems/BallLogicSystem.hpp>
#include <ecs/systems/BrickCollideSystem.hpp>

#include <ecs/components/RenderComponents.hpp>
#include <ecs/components/AudioComponents.hpp>
#include <ecs/components/PhysicsComponents.hpp>
#include <ecs/components/InputComponents.hpp>
#include <ecs/components/BallComponent.hpp>
#include <ecs/components/PlayerComponent.hpp>


#include <breakout/level.hpp>


extern Entity ECS_Game;
extern Entity ECS_Renderer;
extern Entity ECS_Audio;
extern EntityManager ECS_em;
extern ComponentManager ECS_cm;


void ECS_InitSystem();

void ECS_RegisterComponents();
