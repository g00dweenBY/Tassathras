#include "Renderer2D.h"
#include "Graphics/Buffers/VertexBufferLayout.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace Tassathras
{
	Renderer2D::Renderer2DData Renderer2D::m_data;

	void Renderer2D::init()
	{
		std::cout << "renderer2d: init...\n";

		float quadVertices[4 * 2] =
		{
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};
		unsigned int quadIndices[6]
		{
			0, 1, 2,
			2, 3, 0
		};

		m_data.quadVAO = std::make_shared<VertexArray>();

		m_data.quadVBO = std::make_shared<VertexBuffer>(quadVertices, sizeof(quadVertices));

		VertexBufferLayout layout;
		layout.push<float>(2);
		m_data.quadVBO->setLayout(layout);

		m_data.quadVAO->addVertexBuffer(m_data.quadVBO);

		std::shared_ptr<IndexBuffer> quadIBO = std::make_shared<IndexBuffer>(quadIndices, sizeof(quadIndices) / sizeof(unsigned int));
		m_data.quadVAO->setIndexBuffer(quadIBO);

		m_data.flatColorShader = std::make_shared<Shader>("assets/shaders/Base.glsl");
	}

	void Renderer2D::shutdown()
	{
		m_data.quadVAO.reset();
		m_data.flatColorShader.reset();
		std::cout << "renderer: shutdown complete.\n";
	}

	void Renderer2D::beginScene(const glm::mat4& viewProjection)
	{
		m_data.viewProjectionMatrix = viewProjection;
		m_data.flatColorShader->bind();
		m_data.flatColorShader->setMat4("u_viewProjection", m_data.viewProjectionMatrix);
	}

	void Renderer2D::endScene() {}

	void Renderer2D::drawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::mat4(1.0f);

		transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0.0f));

		transform = glm::scale(transform, glm::vec3(size.x, size.y, 1.0f));

		m_data.flatColorShader->setFloat4("u_color", color.r, color.g, color.b, color.a);

		m_data.flatColorShader->setMat4("u_model", transform);

		m_data.quadVAO->bind();
		glDrawElements(GL_TRIANGLES, m_data.quadVAO->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}