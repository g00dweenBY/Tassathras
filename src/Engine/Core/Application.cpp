#include "Application.h"
#include <iostream>



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
void Application::init()
{
	Tassathras::WindowProps props;

	m_window = std::make_unique<Tassathras::Window>(props);
}
void Application::shutdown()
{
	glfwTerminate();
}
void Application::run()
{
	while (!glfwWindowShouldClose(m_window->getNativeWindow()) && m_running )
	{
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_window->onUpdate();
	}
}
