#include "Application.h"
#include "Graphics/Renderers/Renderer2D.h"
#include "Input.h"
#include "Graphics/Texture.h"

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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Renderer2D::init();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_texture = std::make_shared<Texture>("assets/textures/test.png");
	}

	void Application::shutdown()
	{
		Renderer2D::shutdown();
		glfwTerminate();
	}
	void Application::run()
	{
		glm::mat4 projection = glm::ortho(-1.778f, 1.778f, -1.0f, 1.0f);

		while (!m_window->isClosed() && m_running)
		{
			m_window->onUpdate();

;
			if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
			{
				glm::vec2 pos = Input::getMousePosition();
				std::cout << "left click at: " << pos.x << ", " << pos.y << std::endl;
			}
			if (Input::isKeyPressed(GLFW_KEY_W))
				std::cout << "w pressed\n";
			if (Input::isKeyPressed(GLFW_KEY_A))
				std::cout << "A is pressed\n";
			if (Input::isKeyPressed(GLFW_KEY_D))
				std::cout << "D is pressed\n";
			if (Input::isKeyPressed(GLFW_KEY_S))
				std::cout << "S is pressed\n";
			
			glClear(GL_COLOR_BUFFER_BIT); // for clear color;


			Renderer2D::beginScene(projection);
			//Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.3f, 0.3f }, { 0.0f, 0.0f, 1.0f, 1.0f });
			Renderer2D::drawQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, m_texture);

			Renderer2D::endScene();
		}
	}

}