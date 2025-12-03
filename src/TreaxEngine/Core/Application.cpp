#include <glad/glad.h>
#include <iostream>
#include "Application.h"
#include "Graphics/Renderer2D.h"

namespace Tassathras
{
	Application::Application()
	{
		init();
	}

	Application::~Application()
	{
		shutDown();
	}

	void Application::init()
	{
		if (!glfwInit())
		{
			std::cerr << "failed to init glfw!" << std::endl;
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		//create window
		m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
		if (!m_window)
		{
			std::cerr << "failed to create glfw widnow!" << std::endl;
			glfwTerminate();
			return;
		}

		//setting opengl
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);

		std::cout << "openGL v: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "renderer: " << glGetString(GL_RENDERER) << std::endl;
		//init renderer
		Renderer2D::init();
	}
	void Application::run()
	{
		while (m_running && !glfwWindowShouldClose(m_window))
		{
			//deltaTime
			float currentTime = static_cast<float>(glfwGetTime());
			float deltaTime = currentTime - m_lastFrameTime;
			m_lastFrameTime = currentTime;

			//clearing screen
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//update logic
			onUpdate(deltaTime);

			//renderer
			onRender();
			
			//imgui renderer(think next update)
			//onImGuiRender();
			onEvent();
			//swap buffers
			glfwSwapBuffers(m_window);

			//event handling
			glfwPollEvents();

			//check exit
			if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				m_running = false;

			
		}
	}

	void Application::shutDown()
	{
		Renderer2D::shutdown();

		if (m_window)
			glfwDestroyWindow(m_window);
		glfwTerminate();
	}
	
}
