#include <ecs/systems/PlayerLogicSystem.hpp>

#include <glm/glm.hpp>

void PlayerLogicSystem::Update(ComponentManager& cm, uint64_t deltaTime) {
	
	auto& playerArray = cm.GetArray<Player>();

	WindowInfo windowInfo = cm.GetComponent<WindowInfo>(ECS_Renderer);

	int i = 0;
	for (auto& player: playerArray) {
		Entity e = playerArray.GetEntities()[i];

		if (cm.HasComponent<Input>(e) && cm.HasComponent<Velocity>(e) && cm.HasComponent<Transform>(e)) {
			Input& inputComponent = cm.GetComponent<Input>(e);
			Velocity& velocity = cm.GetComponent<Velocity>(e);
			Transform& transform = cm.GetComponent<Transform>(e);

			if (inputComponent.left.active && !inputComponent.right.active) {

				float newPositionX = transform.position.x - 500 * (deltaTime / 1000.0f);
				if (newPositionX > 0) {
					velocity.x = -500;
				}
				else {
					velocity.x = 0;
				}
			}
			else if (inputComponent.right.active && !inputComponent.left.active) {
				float newPositionX = transform.position.x + 500 * (deltaTime / 1000.0f);
				if (newPositionX + transform.size.x < windowInfo.Width) {
					velocity.x = 500;
				}
				else {
					velocity.x = 0;
				}
			}
			else {
				velocity.x = 0;
			}

			// Check collision with ball
			auto& ballArray = cm.GetArray<Ball>();

			int j = 0;
			for (auto& ball : ballArray) {
				Entity ballEntity = ballArray.GetEntities()[j];

				if (cm.HasComponent<Transform>(ballEntity) && cm.HasComponent<Velocity>(ballEntity)) {
					Transform& ballTransform = cm.GetComponent<Transform>(ballEntity);
					Velocity& ballVelocity = cm.GetComponent<Velocity>(ballEntity);

					// Calculamos vector de distancia entre brick y ball
					// distancia de a hacia b = b - a
					glm::vec2 distance = (ballTransform.position + (ballTransform.size / 2.0f)) - (transform.position + (transform.size / 2.0f));

					glm::vec2 overlap = (ballTransform.size + transform.size) / 2.0f - glm::abs(distance);

					// collision only if on both axes
					if (overlap.x > 0 && overlap.y > 0) {

						// Corregir eje y
						ballTransform.position.y = transform.position.y - ballTransform.size.y;

						// Añadir velocidad en eje x en funcion del punto de contacto
						glm::vec2 distance = (ballTransform.position + (ballTransform.size / 2.0f)) - (transform.position + (transform.size / 2.0f));
						
						float percentage = distance.x / (transform.size.x / 2.0f);
						float strength = 10.0f;

					
						glm::vec2 oldVelocity = ballVelocity;

						float velocityFactor = percentage * strength;
						ballVelocity.x = oldVelocity.x + velocityFactor * MAX_BALL_VELOCITY_X;

						// asegurar la direccion de salida en funcion del punto de contacto
						if (percentage > 0) {
							ballVelocity.x = std::max(ballVelocity.x, -ballVelocity.x);
						}
						else {
							ballVelocity.x = std::min(ballVelocity.x, -ballVelocity.x);
						}


						// Capear la velocidad si es mayor que max ball velocity 
						ballVelocity.x = glm::clamp(ballVelocity.x, -MAX_BALL_VELOCITY_X, MAX_BALL_VELOCITY_X);
						ballVelocity.y = glm::clamp(ballVelocity.y, -MAX_BALL_VELOCITY_Y, MAX_BALL_VELOCITY_Y);


						ballVelocity.y = -ballVelocity.y;
						ballVelocity = normalize(ballVelocity) * glm::length(oldVelocity);
						

						// Audio
						if (cm.HasComponent<Sound>(ballEntity)) {
							Sound& ballSound = cm.GetComponent<Sound>(ballEntity);
							ballSound.pitchChanged = true;
							ballSound.pitch = 1;
							ballSound.name = "multhit1";

						}
						if (cm.HasComponent<Sound>(e)) {
							Sound& playerSound = cm.GetComponent<Sound>(e);
							playerSound.play = true;
						}

					}
				}

				j++;
			}
		}
		i++;
	}


}
