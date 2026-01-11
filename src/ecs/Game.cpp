#include <ecs/Game.hpp>

#include <ecs/ECS.hpp>


void ECS_InitGameEntity() {
	ECS_Game= ECS_em.createEntity();

	ECS_cm.AddComponent<GameState>(ECS_Game, GameState::GAME_MENU);
	RawInput rawInputComponent;
	ECS_cm.AddComponent<RawInput>(ECS_Game, rawInputComponent);

}

