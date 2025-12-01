#include<iostream>

#include"Application.h"

namespace TreaxEngine
{
	Application::Application() : m_window(nullptr), m_running(false) {}

	Application::~Application()
	{
		if (m_window) delete m_window;
	}

	void Application::createWindow(const char* title, int width, int height)
	{
		m_window = new Window(title, width, height);
		
	}

	void Application::run(const std::function<void()>& updateFunc)
	{
		if (!m_window)
		{
			std::cerr << "Window not created" << std::endl;
			return;
		}
		m_running = true;

		while (m_running && !m_window->shouldClose())
		{
			m_window->pollEvents();
			
			updateFunc();

			m_window->clear();
			m_window->display();
		}
	}
}