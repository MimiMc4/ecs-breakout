#include <ecs/systems/PhysicsSystem.hpp>


void PhysicsSystem::Update(ComponentManager& cm, uint64_t deltaTime) {
	auto& transformArray= cm.GetArray<Transform>();

	WindowInfo windowInfo = cm.GetComponent<WindowInfo>(ECS_Renderer);

	int i = 0;
	for (Transform& transform : transformArray) {
		Entity e = transformArray.GetEntities()[i];

		if (cm.HasComponent<Velocity>(e)) {

			Velocity& velocity = cm.GetComponent<Velocity>(e);


			transform.position += velocity * (deltaTime / 1000.0f);

			if (cm.HasComponent<Ball>(e)) {
				transform.rotate += -velocity.x / 50;
			}
		}

		i++;
	}
}
