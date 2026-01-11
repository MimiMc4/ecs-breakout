#include <ecs/Renderer.hpp>

#include <ecs/ECS.hpp>
#include <ecs/components/RenderComponents.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>

void ECS_InitRendererEntity(WindowInfo windowInfoComponent) {
	ECS_Renderer= ECS_em.createEntity();

	ECS_cm.AddComponent<WindowInfo>(ECS_Renderer, windowInfoComponent);

	// configure VAO/VBO
	unsigned int VBO;
	unsigned int VAO;
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	MeshComponent mesh;

	mesh.VAO = VAO;
	mesh.VBO = VBO;
	mesh.vertexCount = 6;

	ECS_cm.AddComponent<MeshComponent>(ECS_Renderer, mesh);
}
