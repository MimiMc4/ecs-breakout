#include <ecs/systems/BallLogicSystem.hpp>

#include <glm/glm.hpp>


void BallLogicSystem::Update(ComponentManager& cm) {

	auto& ballArray = cm.GetArray<Ball>();

	WindowInfo windowInfo = cm.GetComponent<WindowInfo>(ECS_Renderer);

	int i = 0;
	for (auto& ballComponent : ballArray) {
		Entity e = ballArray.GetEntities()[i];

		if (cm.HasComponent<Velocity>(e) && cm.HasComponent<Transform>(e) && cm.HasComponent<Input>(e)) {
			Velocity& velocity = cm.GetComponent<Velocity>(e);
			Transform& transform = cm.GetComponent<Transform>(e);
			Input& input = cm.GetComponent<Input>(e);

			if (ballComponent.stuck && input.space.active) {
				if (cm.HasComponent<Transform>(ballComponent.parent) && cm.HasComponent<Velocity>(ballComponent.parent)) {
					Velocity& playerVelocity = cm.GetComponent<Velocity>(ballComponent.parent);
					ballComponent.stuck = false;
					velocity = glm::normalize(playerVelocity + INITIAL_BALL_VELOCITY) * glm::length(INITIAL_BALL_VELOCITY);
				}
			}

			if (ballComponent.stuck) {
				if (cm.HasComponent<Transform>(ballComponent.parent) && cm.HasComponent<Velocity>(ballComponent.parent)) {
					Velocity& playerVelocity = cm.GetComponent<Velocity>(ballComponent.parent);
					velocity = playerVelocity;
				}
			}
			else {
				if (transform.position.x <= 0.0f) {
					velocity.x *= -1;
					transform.position.x = 0.0f;
				}
				else if (transform.position.x + transform.size.x >= windowInfo.Width) {
					velocity.x *= -1;
					transform.position.x = windowInfo.Width - transform.size.x;
				}
				if (transform.position.y <= 0.0f) {
					velocity.y *= -1;
					transform.position.y = 0.0f;
				}
				else if (transform.position.y + transform.size.y >= windowInfo.Height) {
					if (cm.HasComponent<Transform>(ballComponent.parent) && cm.HasComponent<Velocity>(ballComponent.parent)) {
						Transform& playerTransform = cm.GetComponent<Transform>(ballComponent.parent);
						Velocity& playerVelocity = cm.GetComponent<Velocity>(ballComponent.parent);
						velocity = playerVelocity;

						transform.position = glm::vec2(playerTransform.position.x + playerTransform.size.x / 2 - 12, playerTransform.position.y - transform.size.y - 5); 
						ballComponent.stuck = true;

						// Audio
						if (cm.HasComponent<Sound>(ECS_Game)) {
							Sound& lose = cm.GetComponent<Sound>(ECS_Game);
							lose.play = true;
						}
						if (cm.HasComponent<Sound>(e)) {
							Sound& ballSound = cm.GetComponent<Sound>(e);
							if (ballSound.name == "multhit2") {
								ballSound.name = "multhit1";
							}
							ballSound.pitch = 1;
							ballSound.pitchChanged = true;
						}
					}
				}
			}
		}
		i++;
	}



}