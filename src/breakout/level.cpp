#include <breakout/level.hpp>



#include <fstream>
#include <sstream>



void breakout::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight) {

	unsigned int tileCode;
	unsigned int tileCount = 0;
	unsigned int lineSize = 0;


	std::string line;
	std::ifstream fstream(file);
	//std::vector<std::vector<unsigned int>> tileData;
	std::array<unsigned int, MAX_BRICKS> tileData;
	if (fstream)
	{
		while (std::getline(fstream, line)) // read each line from level file
		{
			std::istringstream sstream(line);
			while (sstream >> tileCode) // read each word separated by spaces
				tileData[tileCount++] = tileCode;

			if (lineSize < tileCount - lineSize)
				lineSize = tileCount - lineSize;
		}


		if(tileCount > 0)
			breakout::Init(tileData, tileCount, 15, levelWidth, levelHeight);
	}
}

void breakout::Init(std::array<unsigned int, MAX_BRICKS> tileData, unsigned int tileCount, unsigned int lineSize, unsigned int levelWidth, unsigned int levelHeight) {


	BrickArray brickArray;

	// calculate dimensions
	unsigned int height = tileCount / lineSize;
	unsigned int width = lineSize;
	float unit_width = levelWidth / static_cast<float>(width);
	float unit_height = levelHeight / static_cast<float>(height);
	// initialize level tiles based on tileData		
	for (unsigned int i = 0; i < tileCount ; i++)
	{

		unsigned int x = i % lineSize;
		unsigned int y = i / lineSize;

		Entity newBrickEntity = ECS_em.createEntity();
		// check block type from level data (2D level array)
		if (tileData[i] == 1) { //solid
			Transform transform;
			transform.position = glm::vec2(unit_width * x, unit_height * y);
			transform.size = glm::vec2(unit_width, unit_height);
			ECS_cm.AddComponent<Transform>(newBrickEntity, transform);

			TextureComponent texture;
			texture.TextureName = "block_solid";
			texture.color = glm::vec3(0.8f, 0.8f, 0.6f);
			//texture.color = glm::vec3(1.0f, 1.0f, 1.0f);
			ECS_cm.AddComponent<TextureComponent>(newBrickEntity, texture);

			
			Brick brick;
			brick.isSolid = true;
			ECS_cm.AddComponent<Brick>(newBrickEntity, brick);

		} else if (tileData[i] > 1)
		{
			glm::vec3 color = glm::vec3(1.0f); // original: white
			if (tileData[i] == 2)
				color = glm::vec3(0.5f, 0.8f, 0.5f);
			else if (tileData[i] == 3)
				color = glm::vec3(0.2f, 0.8f, 0.5f);
			else if (tileData[i] == 4)
				color = glm::vec3(0.5f, 0.5f, 0.8f);
			else if (tileData[i] == 5)
				color = glm::vec3(0.8f, 0.5f, 0.5f);

			Transform transform;
			transform.position = glm::vec2(unit_width * x, unit_height * y);
			transform.size = glm::vec2(unit_width, unit_height);
			ECS_cm.AddComponent<Transform>(newBrickEntity, transform);

			TextureComponent texture;
			texture.TextureName = "block";
			texture.color = color;
			ECS_cm.AddComponent<TextureComponent>(newBrickEntity, texture);

			Brick brick;
			ECS_cm.AddComponent<Brick>(newBrickEntity, brick);
		}

		brickArray.Bricks[i] = newBrickEntity;
		brickArray.count++;
	}

	if (ECS_cm.HasComponent<BrickArray>(ECS_Game))
		ECS_cm.RemoveComponent<BrickArray>(ECS_Game);

	ECS_cm.AddComponent<BrickArray>(ECS_Game, brickArray);

	// draw background
	/*
	Entity background = ECS_em.createEntity();

	Transform bgTransform;
	bgTransform.size.x = static_cast<float>(ECS_cm.GetComponent<WindowInfo>(ECS_Renderer).Width);
	bgTransform.size.y = static_cast<float>(ECS_cm.GetComponent<WindowInfo>(ECS_Renderer).Height);
	ECS_cm.AddComponent<Transform>(background, bgTransform);

	TextureComponent bgTexture;
	bgTexture.TextureName = "background";
	bgTexture.layer = -1;
	ECS_cm.AddComponent<TextureComponent>(background, bgTexture);

	*/

}
