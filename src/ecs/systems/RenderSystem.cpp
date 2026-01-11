#include  <ecs/systems/RenderSystem.hpp>

#include <ecs/components/RenderComponents.hpp>
#include <ecs/components/PhysicsComponents.hpp>

#include <render/ResourceManager.hpp>
#include <render/Shader.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <iostream>

void RenderSystem::Update(ComponentManager& cm) {

	auto& textureArray = cm.GetArray<TextureComponent>();
	Shader shader = ResourceManager::GetShader(cm.GetComponent<ShaderComponent>(ECS_Renderer).ShaderName);
	shader.Use();


	// Iteramos por las texturas
	int i = 0;
	for (TextureComponent& textureComponent : textureArray) {
		if (!textureComponent.isVisible) {
			i++;
			continue;
		}

		Texture2D texture = ResourceManager::GetTexture(textureComponent.TextureName);
		Entity e = textureArray.GetEntities()[i];
		if (cm.HasComponent<Transform>(e)) {
			Transform transform = cm.GetComponent<Transform>(e);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(transform.position, textureComponent.layer));

			model = glm::translate(model, glm::vec3(0.5f * transform.size.x, 0.5f * transform.size.y, 0.0f));
			model = glm::rotate(model, glm::radians(transform.rotate), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.5f * transform.size.x, -0.5f * transform.size.y, 0.0f));

			model = glm::scale(model, glm::vec3(transform.size, 1.0f));

			shader.SetMatrix4("model", model);
			shader.SetVector3f("spriteColor", textureComponent.color);

			glActiveTexture(GL_TEXTURE0);
			texture.Bind();

			glBindVertexArray(cm.GetComponent<MeshComponent>(ECS_Renderer).VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

		}
		i++;
	}
}