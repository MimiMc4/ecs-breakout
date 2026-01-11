#pragma once

#include <glm/glm.hpp>

#include <string>

struct ShaderComponent{
	std::string ShaderName;
};


struct TextureComponent{
	std::string TextureName;
	glm::vec3 color = glm::vec3(1.0f);
	bool isVisible = true;
	// "eje z" -10 capa mas cercana, 10 mas lejana
	int layer = 0;	
};

// Componente para geometría (VAO/VBO)
struct MeshComponent {
	unsigned int VAO;
	unsigned int VBO;
	uint32_t vertexCount;
};
