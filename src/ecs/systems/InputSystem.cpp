#include <ecs/systems/InputSystem.hpp>


void InputSystem::Update(ComponentManager& cm) {
	auto& inputArray = cm.GetArray<Input>();
	int i = 0;
	for (auto& inputComponent: inputArray) {
		RawInput& raw = cm.GetComponent<RawInput>(ECS_Game);

		// simplemente actualizamos el booleano en funcion del raw input
		inputComponent.right.active = raw.Keys[inputComponent.right.keycode];
		inputComponent.left.active = raw.Keys[inputComponent.left.keycode];
		inputComponent.space.active = raw.Keys[inputComponent.space.keycode];
	}
}
