#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Buffers/VertexArray.h"
#include "Graphics/Buffers/VertexBuffer.h"
#include "Graphics/Buffers/IndexBuffer.h"

#include <glm/glm.hpp>
#include <memory>

namespace Tassathras
{
	class Renderer2D
	{
	public:

		static void init();

		static void shutdown();

		static void beginScene(const glm::mat4& viewProjection);

		static void endScene();

		static void drawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);

	private:
		struct Renderer2DData
		{
			std::shared_ptr<VertexArray> quadVAO;
			std::shared_ptr<VertexBuffer> quadVBO;
			std::shared_ptr<Shader> flatColorShader;
			glm::mat4 viewProjectionMatrix;

		};
		static Renderer2DData m_data;

		Renderer2D() = delete;
	};
}
