#include <ecs/systems/BrickCollideSystem.hpp>

#include <glm/glm.hpp>


// Producto punto con las direcciones cardinales
// El que sea mayor es la direccion desde la que golpea la bola
// 0 - arriba, 1 - derecha, 2 - abajo, 3 - izquierda
BrickCollideSystem::Direction getDirection(glm::vec2 target) {

	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};

	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (BrickCollideSystem::Direction) best_match;
}

int countSolid(ComponentManager& cm) {
	auto& brickArray = cm.GetArray<Brick>();
	int count = 0;
	for (auto& brick : brickArray) {
		if (brick.isSolid) {
			count++;
		}
	}
	return count;
}

void BrickCollideSystem::Update(ComponentManager& cm) {

	static int numberOfBricks = ECS_cm.GetArray<Brick>().GetCount();
	static int numerOfBreakableBricksLeft = numberOfBricks - countSolid(cm);

	auto& brickArray = cm.GetArray<Brick>();

	auto& ballArray = cm.GetArray<Ball>();

	int i = 0;
	for (auto& brick : brickArray) {
		if (brick.destroyed) {
			i++;
			continue;
		}
		Entity e = brickArray.GetEntities()[i];

		if (cm.HasComponent<Transform>(e)) {
			Transform& brickTransform = cm.GetComponent<Transform>(e);

			int j = 0;
			for (auto& ball : ballArray) {
				Entity ballEntity = ballArray.GetEntities()[j];

				if (cm.HasComponent<Transform>(ballEntity) && cm.HasComponent<Velocity>(ballEntity)) {
					Transform& ballTransform = cm.GetComponent<Transform>(ballEntity);
					Velocity& ballVelocity = cm.GetComponent<Velocity>(ballEntity);


					// Calculamos vector de distancia entre brick y ball
					// distancia de a hacia b => b - a
					glm::vec2 distance = (ballTransform.position + (ballTransform.size / 2.0f)) - (brickTransform.position + (brickTransform.size / 2.0f));

					glm::vec2 overlap = (ballTransform.size + brickTransform.size) / 2.0f - glm::abs(distance);

					// collision only if on both axes
					if (overlap.x > 0 && overlap.y > 0) {
						// Determine primary collision direction (smallest overlap)
						if (overlap.x < overlap.y) {
							// Horizontal collision
							if (distance.x > 0) { // Right
								ballTransform.position.x = brickTransform.position.x + brickTransform.size.x;
								ballVelocity.x = glm::abs(ballVelocity.x); // Ensure bounce to right
							} else { // Left
								ballTransform.position.x = brickTransform.position.x - ballTransform.size.x;
								ballVelocity.x = -glm::abs(ballVelocity.x); 
							}
						} else {
							// Vertical collision
							if (distance.y > 0) { 
								ballTransform.position.y = brickTransform.position.y + brickTransform.size.y;
								ballVelocity.y = glm::abs(ballVelocity.y); 
							} else { 
								ballTransform.position.y = brickTransform.position.y - ballTransform.size.y;
								ballVelocity.y = -glm::abs(ballVelocity.y); 
							}
						}

						// desactivar bloque si se puede romper
						if (!brick.isSolid) {
							numerOfBreakableBricksLeft--;
							if (numerOfBreakableBricksLeft == 0) {
								GameState& gameState = cm.GetComponent<GameState>(ECS_Game);
								gameState = GameState::GAME_WIN;
							}

							brick.destroyed = true;
							TextureComponent& texture = cm.GetComponent<TextureComponent>(e);
							texture.isVisible = false;

							// efecto de sonido
							if (cm.HasComponent<Sound>(ballEntity)) {
								Sound& ballSound = cm.GetComponent<Sound>(ballEntity);

								ballSound.play = true;
								ballSound.pitchChanged = true;
								ballSound.pitch += 0.1;
								if (ballSound.pitch > 1.5) {
									ballSound.name = "multhit2";
								}
							}
						}
					}
				}
				j++;
			}
		}
		i++;
	}
}

