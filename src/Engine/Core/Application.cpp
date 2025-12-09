#include "Application.h"
#include "Graphics/Renderers/Renderer2D.h"
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

		Renderer2D::init();
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
			
			Renderer2D::beginScene(projection);
			Renderer2D::drawQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f });
			Renderer2D::drawQuad({ 0.5f, 0.5f }, { 0.3f, 0.3f }, { 0.0f, 0.0f, 1.0f, 1.0f });

			Renderer2D::endScene();
		}
	}

}