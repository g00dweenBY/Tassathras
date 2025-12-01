#include<iostream>
#include "Window.h"

namespace Tassathras
{
	Window::Window(const char* title, int width, int height)
	{
		if (!glfwInit())
		{
			std::cerr << "failed to init glfw" << std::endl;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

		if (!m_window)
		{
			std::cerr << "failed to create glfw window" << std::endl;
			glfwTerminate();
			exit(-1);
		}

		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "failed to init glad" << std::endl;
			exit(-1);
		}

		glViewport(0, 0, width, height);

		//vsync
		glfwSwapInterval(1);

		std::cout << "opengl renderer : " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "opengl version: " << glGetString(GL_VERSION) << std::endl;
	}
	Window::~Window()
	{
		if (m_window) glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Window::pollEvents()
	{
		glfwPollEvents();
	}
	void Window::clear()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void Window::display()
	{
		glfwSwapBuffers(m_window);
	}

	bool Window::shouldClose() const
	{
		return glfwWindowShouldClose(m_window);
	}
}