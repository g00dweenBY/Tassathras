#include <glad/glad.h>
#include <iostream>
#include <array>
#include "Renderer2D.h"
#include "Shader.h"
#include "Camera.h"

namespace Tassathras
{
	uint32_t Renderer2D::m_quadVA = 0;
	uint32_t Renderer2D::m_quadVB = 0;
	uint32_t Renderer2D::m_quadIB = 0;

	std::unique_ptr<Renderer2D::Vertex[]> Renderer2D::m_vertexBufferBase = nullptr;
	Renderer2D::Vertex* Renderer2D::m_vertexBufferPtr = nullptr;
	uint32_t Renderer2D::m_indexCount = 0;

	std::unique_ptr<Shader> Renderer2D::m_Shader = nullptr;
	glm::mat4 Renderer2D::m_viewProjectionMatrix = glm::mat4(1.0f);

	Renderer2D::Statistics Renderer2D::m_statistics;
	bool Renderer2D::m_initialized = false;

	void Renderer2D::init()
	{
		if (m_initialized)
		{
			std::cout << "Render already initialized!" << std::endl;
			return;
		}

		//1. create vertexArray and buffers
		glGenVertexArrays(1, &m_quadVA);
		glBindVertexArray(m_quadVA);

		//2. create and setting buffer
		glGenBuffers(1, &m_quadVB);
		glBindBuffer(GL_ARRAY_BUFFER, m_quadVB);
		glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		//3. Settings attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

		//4. Create index buffer
		std::vector<uint32_t> indices;
		indices.resize(MAX_INDICES);

		for (uint32_t i = 0, offset = 0; i < MAX_INDICES; i += 6, offset += 4)
		{
			
			//first triangle
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			//second triangle
			indices[i + 3] = offset + 3;
			indices[i + 4] = offset + 4;
			indices[i + 5] = offset + 5;

			offset += 4;
		}
		glGenBuffers(1, &m_quadIB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

		//create shaders Here(ну да, он же маленький, похуй)
		const char* vertexShaderSource = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec4 a_color;
            
            uniform mat4 u_viewProjection;
            
            out vec4 v_Color;
            
            void main()
            {
                v_Color = a_color;
                gl_Position = u_viewProjection * vec4(a_Position, 1.0);
            }
        )";

		const char* fragmentShaderSource = R"(
            #version 330 core
            
            in vec4 v_color;
            out vec4 fragColor;
            
            void main()
            {
                FragColor = v_color;
            }
        )";

		m_Shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);

		//6. initialized buffer
		m_vertexBufferBase = std::make_unique<Vertex[]>(MAX_VERTICES);

		//7. starting first batch
		startBatch();
		m_initialized = true;
		std::cout << "Renderer initialized successfully!" << std::endl;
	}

	void Renderer2D::shutdown()
	{
		if (!m_initialized)
			return;

		glDeleteVertexArrays(1, &m_quadVA);
		glDeleteBuffers(1, &m_quadVB);
		glDeleteBuffers(1, &m_quadIB);

		m_Shader.reset();
		m_vertexBufferBase.reset();

		m_initialized = false;
		std::cout << "Renderer complete the work" << std::endl;
	}
	void Renderer2D::beginScene(const Camera& camera)
	{
		m_viewProjectionMatrix = camera.getViewProjectionMatrix();
		startBatch();
	}

	void Renderer2D::endScene()
	{
		flush();
	}

	void Renderer2D::drawQuad(
		const glm::vec3& pos,
		const glm::vec2& size,
		float rotationRadians,
		const glm::vec4& color
	)
	{
		if (m_indexCount >= MAX_INDICES)
			flushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);

		if (rotationRadians != 0.0f)
			transform = glm::rotate(transform, rotationRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		drawQuad(transform, color);
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		if (m_indexCount >= MAX_INDICES)
			flushAndReset();

		constexpr std::array<glm::vec4, 4> quadVertexPositions =
		{
			glm::vec4{ -0.5f, -0.5f, 0.0f, 1.0f },
			glm::vec4{  0.5f, -0.5f, 0.0f, 1.0f },
			glm::vec4{  0.5f,  0.5f, 0.0f, 1.0f },
			glm::vec4{ -0.5f,  0.5f, 0.0f, 1.0f }
		};
		//====================================================================================
		for (int i = 0; i < 4; ++i)
		{
			glm::vec4 transformedPosition = transform * quadVertexPositions[i];
			m_vertexBufferPtr->position = glm::vec3(transformedPosition);
			m_vertexBufferPtr->color = color;
			m_vertexBufferPtr++;
		}
		m_indexCount += 6;
		m_statistics.quadCount++;
		//====================================================================================
	}

	void Renderer2D::drawRotateQuad(
		const glm::vec2& pos,
		const glm::vec2& size,
		float rotationDegrees,
		const glm::vec4& color
	)
	{
		drawQuad(
			glm::vec3(pos, 0.0f),
			size,
			glm::radians(rotationDegrees),
			color
		);
	}

	void Renderer2D::startBatch()
	{
		m_vertexBufferPtr = m_vertexBufferBase.get();
		m_indexCount = 0;
	}

	void Renderer2D::flush()
	{
		if (m_indexCount == 0) return;

		size_t vertexCount = m_vertexBufferPtr - m_vertexBufferBase.get();
		uint32_t dataSize = static_cast<uint32_t>(vertexCount * sizeof(Vertex));


		glBindBuffer(GL_ARRAY_BUFFER, m_quadVB);
		glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, m_vertexBufferBase.get());

		m_Shader->bind();
		m_Shader->setMat4("u_viewProjection", m_viewProjectionMatrix);

		glBindVertexArray(m_quadVA);
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);

		m_statistics.drawCalls++;
	}

	void Renderer2D::flushAndReset()
	{
		flush();
		startBatch();
	}

	void Renderer2D::resetStatistics()
	{
		m_statistics = {};
	}

	Renderer2D::Statistics Renderer2D::getStatistics()
	{
		return m_statistics;
	}
}