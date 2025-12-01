
#include "Renderer2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace TreaxEngine
{
	unsigned int Renderer2D::s_VAO = 0;
	unsigned int Renderer2D::s_VBO = 0;
	Shader* Renderer2D::s_Shader = nullptr;

	void Renderer2D::init()
	{
		float vertices[] =
		{
			//x,y
			 0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, -0.5f,
			 0.0f, -0.5f, -0.5f,
			 
		};

		unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

		glGenVertexArrays(1, &s_VAO);
		glGenBuffers(1, &s_VBO);

		glBindVertexArray(s_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		s_Shader = new Shader("assets/shaders/quad.vert", "assets/shaders/quad.frag");
	}

	void Renderer2D::beginScene(const glm::mat4& projection)
	{
		s_Shader->bind();
		s_Shader->setMat4("u_ViewProjection", projection);
	}


	void Renderer2D::drawQuad(
		const glm::vec2& pos,
		const glm::vec2& size,
		float rotation,
		const glm::vec4& color
	)
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		model = glm::scale(model, glm::vec3(size, 1.0f));

		s_Shader->setMat4("u_Model", model);
		s_Shader->setVec4("u_Color", color);

		glBindVertexArray(s_VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindVertexArray(0);
	}


	void Renderer2D::endScene()
	{
		s_Shader->unbind();
	}

}