#include <ecs/ECS.hpp>

Entity ECS_Game;
Entity ECS_Renderer;
Entity ECS_Audio;
EntityManager ECS_em = EntityManager();
ComponentManager ECS_cm = ComponentManager();

void ECS_InitSystem() {
	ECS_RegisterComponents();

	ECS_InitGameEntity();

}


void ECS_RegisterComponents() {

	ECS_cm.RegisterComponent<GameState>();
	ECS_cm.RegisterComponent<RawInput>();
	ECS_cm.RegisterComponent<WindowInfo>();


	ECS_cm.RegisterComponent<TextureComponent>();
	ECS_cm.RegisterComponent<ShaderComponent>();
	ECS_cm.RegisterComponent<MeshComponent>();

	ECS_cm.RegisterComponent<Sound>();

	ECS_cm.RegisterComponent<Transform>();
	ECS_cm.RegisterComponent<Velocity>();

	ECS_cm.RegisterComponent<BrickArray>();
	ECS_cm.RegisterComponent<Brick>();

	ECS_cm.RegisterComponent<Input>();

	ECS_cm.RegisterComponent<Player>();
	ECS_cm.RegisterComponent<Ball>();
}
