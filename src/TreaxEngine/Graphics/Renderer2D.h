#pragma once
#include "Shader.h"
#include<glm/glm.hpp>


namespace TreaxEngine
{
	class Renderer2D
	{
	public:
		static void init();

		static void drawQuad(
			const glm::vec2& pos,
			const glm::vec2& size,
			float rotation,
			const glm::vec4& color
		);

		static void beginScene(const glm::mat4& viewProj);
		static void endScene();

	private:
		static unsigned int s_VAO;
		static unsigned int s_VBO;

		static Shader* s_Shader;

		
	};
}