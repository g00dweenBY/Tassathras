#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "Graphics/Texture.h"

namespace Tassathras
{
	struct TransformComponent
	{
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec2 scale = { 1.0f, 1.0f };
		float rotation = 0.0f //radian

		glm::mat4 getTransform() const
		{
			glm::mat4 transform = glm::mat4(1.0f);

			transform = glm::translate(transform, position);
			
			transform = glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f));

			transform = glm::scale(transform, glm::vec3(scale.x, scale.y, 1.0f));

			return transform;
		}
	};

	struct spriteRendererComponent
	{
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		std::share
	};
}
