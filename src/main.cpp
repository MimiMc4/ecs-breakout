
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL3/SDL.h>

#include <miniaudio.h>
#include <stb_image.h>

#include <ecs/ECS.hpp>

#include <render/ResourceManager.hpp>
#include <render/Shader.hpp>
#include <render/Texture.hpp>


#include <thread>
#include <iostream>


constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;
bool isLargeWindow = false;

const SDL_FRect WINDOW_SIZE_SMALL = { 0, 0, 800, 600 };
const SDL_FRect WINDOW_SIZE_LARGE = { 0, 0, 1200, 900 };

const char* APP_NAME = "Breakout";
const char* APP_VERSION = "0.0.0.0";

const int TARGET_FPS = 60;
const uint64_t NS_PER_FRAME = 1e9 / TARGET_FPS;

Entity player;

Entity end_image;


// Audio
ma_result result;
ma_engine engine;
ma_sound sound;

void ToggleWindowSize(SDL_Window* window) {
	isLargeWindow = !isLargeWindow;

	if (isLargeWindow) {
		SDL_SetWindowSize(window, WINDOW_SIZE_LARGE.w, WINDOW_SIZE_LARGE.h);
	} else {
		SDL_SetWindowSize(window, WINDOW_SIZE_SMALL.w, WINDOW_SIZE_SMALL.h);
	}

	// Actualizar la vista de OpenGL
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);

}

void init() {
	// Setup de SDL
	SDL_SetAppMetadata(APP_NAME, APP_VERSION, NULL);

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
	}



	// Setup de OpenGL
	SDL_Window* window = SDL_CreateWindow(APP_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(0);


	// Setup de GLAD
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		SDL_Log("Failed to initialize GLAD");
	}

	// OpenGL configuration
	// --------------------
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	glEnable(GL_MULTISAMPLE);


	// Window icon
	int width, height, channels;
	unsigned char* icon_data = stbi_load("assets/textures/balatro.png", &width, &height, &channels, STBI_rgb_alpha);

	if (icon_data) {
		// Crear superficie SDL desde los datos de la imagen
		SDL_Surface* icon = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGBA32, (void*)icon_data, width * 4);

		if (icon) {
			SDL_SetWindowIcon(window, icon);
			SDL_DestroySurface(icon);
		} else {
			SDL_Log("Error al crear superficie del icono: %s", SDL_GetError());
		}

		stbi_image_free(icon_data);
	} else {
		SDL_Log("No se pudo cargar el icono: %s", stbi_failure_reason());
	}

	// initialize game
	// ---------------
	ECS_InitSystem();

	// audio
	ResourceManager::initAudio();
	ECS_Audio = ECS_em.createEntity();

	//load audio
	ResourceManager::LoadSound("assets/audio/balatro.wav", "balatro", true);
	ResourceManager::LoadSound("assets/audio/multhit1.wav", "multhit1", false);
	ResourceManager::LoadSound("assets/audio/multhit2.wav", "multhit2", false);
	ResourceManager::LoadSound("assets/audio/card1.wav", "card1", false);
	ResourceManager::LoadSound("assets/audio/glass3.wav", "glass3", false);
	ResourceManager::LoadSound("assets/audio/holo1.wav", "holo1", false);


	Sound bgm;
	bgm.name = "balatro";
	bgm.play = true;
	bgm.volume = 0.5;
	bgm.volumeChanged = true;
	ECS_cm.AddComponent<Sound>(ECS_Audio, bgm);

	Sound lose;
	lose.name = "glass3";
	lose.volume = 0.8;
	lose.volumeChanged = true;
	ECS_cm.AddComponent<Sound>(ECS_Game, lose);



	WindowInfo windowInfoComponent;
	windowInfoComponent.Height = SCREEN_HEIGHT;
	windowInfoComponent.Width = SCREEN_WIDTH;
	windowInfoComponent.window = window;
	ECS_InitRendererEntity(windowInfoComponent);

	// load shaders
	ResourceManager::LoadShader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader("assets/shaders/background.vert", "assets/shaders/background.frag", nullptr, "background");

	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowInfoComponent.Width),
			static_cast<float>(windowInfoComponent.Height), 0.0f, -10.0f, 10.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	ResourceManager::GetShader("background").SetMatrix4("projection", projection);


	// set default render
	ShaderComponent shaderComponent;
	shaderComponent.ShaderName = "sprite";
	ECS_cm.AddComponent<ShaderComponent>(ECS_Renderer, shaderComponent);

	// load textures
	ResourceManager::LoadTexture("assets/textures/background.jpg", false, "background");
	ResourceManager::LoadTexture("assets/textures/balatro.png", true, "face");
	ResourceManager::LoadTexture("assets/textures/balatro.png", true, "block");
	ResourceManager::LoadTexture("assets/textures/balatro.png", true, "block_solid");
	ResourceManager::LoadTexture("assets/textures/balatro.png", true, "paddle");
	ResourceManager::LoadTexture("assets/textures/ending.png", true, "ending");

	// load levels
	breakout::Load("assets/levels/one.lvl", static_cast<float>(windowInfoComponent.Width), static_cast<float>(windowInfoComponent.Height) / 2.0f);

	// create player
	player = ECS_em.createEntity();

	Transform playerTransform;
	playerTransform.size = glm::vec2(100.0f, 20.0f);
	playerTransform.position =	glm::vec2( windowInfoComponent.Width / 2.0f - playerTransform.size.x / 2.0f,
		windowInfoComponent.Height - playerTransform.size.y	);
	ECS_cm.AddComponent<Transform>(player, playerTransform);

	TextureComponent playerTexture;
	playerTexture.TextureName = "paddle";
	ECS_cm.AddComponent<TextureComponent>(player, playerTexture);

	Velocity playerVelocity = glm::vec2(0.0f, 0.0f);
	ECS_cm.AddComponent<Velocity>(player, playerVelocity);

	Input playerInput;
	ECS_cm.AddComponent<Input>(player, playerInput);

	Sound playerSound;
	playerSound.name = "card1";
	ECS_cm.AddComponent<Sound>(player, playerSound);

	Player playerComponent;
	ECS_cm.AddComponent<Player>(player, playerComponent);

	// create ball
	Entity ball = ECS_em.createEntity();

	Transform ballTransform;
	ballTransform.size = glm::vec2(25, 25);
	ballTransform.position =	glm::vec2(playerTransform.position.x + playerTransform.size.x / 2 - 12, playerTransform.position.y - ballTransform.size.y - 5);
	ECS_cm.AddComponent<Transform>(ball, ballTransform);

	TextureComponent ballTexture;
	ballTexture.TextureName = "face";
	ballTexture.layer = 1;
	ECS_cm.AddComponent<TextureComponent>(ball, ballTexture);

	Sound ballSound;
	ballSound.name = "multhit1";
	ECS_cm.AddComponent<Sound>(ball, ballSound);

	Velocity ballVelocity = glm::vec2(0,0);
	ECS_cm.AddComponent<Velocity>(ball, ballVelocity);

	Input ballInput;
	ECS_cm.AddComponent<Input>(ball, ballInput);

	Ball ballComponent;
	ballComponent.stuck = true;
	ballComponent.parent = player;
	ECS_cm.AddComponent<Ball>(ball, ballComponent);

	// create end image
	end_image = ECS_em.createEntity();

	TextureComponent endImageTexture;
	endImageTexture.TextureName = "ending";
	endImageTexture.isVisible = false;
	endImageTexture.layer = 3;
	ECS_cm.AddComponent<TextureComponent>(end_image, endImageTexture);

	Transform endImageTransform;
	endImageTransform.position = glm::vec2(0.0, 0.0);
	endImageTransform.size.x = static_cast<float>(ECS_cm.GetComponent<WindowInfo>(ECS_Renderer).Width);
	endImageTransform.size.y = static_cast<float>(ECS_cm.GetComponent<WindowInfo>(ECS_Renderer).Height);
	ECS_cm.AddComponent<Transform>(end_image, endImageTransform);
}

void handle_events() {
	SDL_Event event;
	static bool both = false;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_EVENT_QUIT:

			std::cout << "\nFinalizando....\n";
			ECS_cm.GetComponent<GameState>(ECS_Game) = GameState::GAME_END;
			break;

		case SDL_EVENT_KEY_DOWN:
			if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
				std::cout << "\nESC detectado\n";
				std::cout << "\nFinalizando....\n";
				ECS_cm.GetComponent<GameState>(ECS_Game) = GameState::GAME_END;
			} else if (event.key.scancode == SDL_SCANCODE_KP_PLUS) {
				if(ECS_cm.HasComponent<WindowInfo>(ECS_Renderer))
					ToggleWindowSize(ECS_cm.GetComponent<WindowInfo>(ECS_Renderer).window);
			}
			else {
				SDL_Keycode key = SDL_GetKeyFromScancode(event.key.scancode, SDL_KMOD_NONE, false);
				ECS_cm.GetComponent<RawInput>(ECS_Game).Keys[key] = true;
			}
			break;
		case SDL_EVENT_KEY_UP:
			SDL_Keycode key = SDL_GetKeyFromScancode(event.key.scancode, SDL_KMOD_NONE, false);
			ECS_cm.GetComponent<RawInput>(ECS_Game).Keys[key]= false;
			break;
		}
	}
}

void clean() {
	ResourceManager::Clear();
	SDL_Quit();
}


int main() {
	init();


	uint64_t deltaTime = 0;
	uint64_t lastFrame = 0;



	while (ECS_cm.GetComponent<GameState>(ECS_Game) != GameState::GAME_END) {

		// calcular delta time
		// --------------------
		uint64_t frame_start_time = SDL_GetTicksNS();
		deltaTime = (frame_start_time - lastFrame) / 1000000.0f;
		lastFrame = frame_start_time;

		// InputSystem
		// ------
		handle_events();

		

		// Updates
		// ------
		InputSystem::Update(ECS_cm);
		BrickCollideSystem::Update(ECS_cm);
		PlayerLogicSystem::Update(ECS_cm, NS_PER_FRAME / 1000000);
		BallLogicSystem::Update(ECS_cm);
		PhysicsSystem::Update(ECS_cm, NS_PER_FRAME / 1000000);


		// WIN

		if (ECS_cm.GetComponent<GameState>(ECS_Game) == GameState::GAME_WIN) {
			TextureComponent& endImageTexture = ECS_cm.GetComponent<TextureComponent>(end_image);
			endImageTexture.isVisible = true;
		}

		//Audio
		// ------
		AudioSystem::Update(ECS_cm);


		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);		// Limpiar el buffer de profundidad


		// Shader fondo
		glDisable(GL_DEPTH_TEST);
		
		ResourceManager::GetShader("background").Use().SetFloat("iTime", frame_start_time / 1000000000.0f);

		WindowInfo& windowInfo = ECS_cm.GetComponent<WindowInfo>(ECS_Renderer);
		ResourceManager::GetShader("background").SetVector2f("iResolution", glm::vec2(windowInfo.Width, windowInfo.Height));


		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(windowInfo.Width, windowInfo.Height, 1.0f));
		ResourceManager::GetShader("background").SetMatrix4("model", model);

		
		glBindVertexArray(ECS_cm.GetComponent<MeshComponent>(ECS_Renderer).VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
		


		RenderSystem::Update(ECS_cm);

		SDL_GL_SwapWindow(ECS_cm.GetComponent<WindowInfo>(ECS_Renderer).window);


		// Control de tiempo preciso 
		// -------------------------
		// sleep durante la mayor parte del tiempo
		uint64_t time_elapsed = SDL_GetTicksNS() - frame_start_time;
		uint64_t time_left = NS_PER_FRAME - time_elapsed;

		uint64_t sleep_time = time_left - 1000000;
		if (sleep_time > 0) {
			SDL_DelayPrecise(sleep_time);
		}

		// Espera activa los ultimos milisegundos
		uint64_t exit_time = NS_PER_FRAME + frame_start_time;
		while (SDL_GetTicksNS() < exit_time) {
			//std::this_thread::yield();
		}

		double delta_ms = (SDL_GetTicksNS() - frame_start_time) / 1000000.0;
		std::cout << "\rFPS: " << 1000.0 / delta_ms;

	}

	clean();

	return 0;
}
