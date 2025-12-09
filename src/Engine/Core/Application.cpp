#include "Application.h"
#include "Graphics/Buffers/VertexBuffer.h"
#include "Graphics//Buffers/VertexArray.h"
#include<GLFW/glfw3.h>
#include<glm/gtc/matrix_transform.hpp>


namespace Tassathras
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		if (s_instance != nullptr)
		{
			std::cerr << "application already exissts\n";
			return;
		}
		s_instance = this;
		init();
	}

	Application::~Application()
	{
		shutdown();
		s_instance = nullptr;
	}
	//===============================================================
	// life cycle
	//===============================================================
	void Application::init()
	{
		m_window = std::make_unique<Tassathras::Window>(Tassathras::WindowProps());

		float vertices[4 * 2] =
		{
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};
		unsigned int indices[6]
		{
			0, 1, 2,
			2, 3, 0
		};

		m_vertexArray = std::make_shared<VertexArray>();

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));

		VertexBufferLayout layout;
		layout.push<float>(2); // 2float for pos(x,y)

		vertexBuffer->setLayout(layout);
		m_vertexArray->addVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

		m_vertexArray->setIndexBuffer(indexBuffer);

		m_shader = std::make_shared<Shader>("assets/shaders/Base.glsl");
		m_shader->bind();

		glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

		m_shader->setMat4("u_viewProjection", projection);
		m_shader->setFloat4("u_color", 0.9f, 0.2f, 0.2f, 1.0f);
	}
	void Application::shutdown()
	{
		glfwTerminate();
	}
	void Application::run()
	{
		while (!glfwWindowShouldClose(m_window->getNativeWindow()) && m_running)
		{

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shader->bind();
			m_vertexArray->bind();

			glDrawElements(
				GL_TRIANGLES,
				m_vertexArray->getIndexBuffer()->getCount(),
				GL_UNSIGNED_INT,
				nullptr
			);

			m_window->onUpdate();
		}
	}

}