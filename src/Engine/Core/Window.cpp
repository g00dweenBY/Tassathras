#include "Window.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Tassathras
{
	//working container for state and API GLFW
	struct WindowData
	{
		std::string title;
		unsigned int width = 0;
		unsigned int height = 0;
		//next here callback's GLFW
	};
	static bool s_GLFWInit = false;
	// === callback functions ==
	void GLFWErrorCallback(int error, const char* description)
	{
		std::cerr << "glfw error (" << error << "): " << description << std::endl;
	}
	void windowResizeCallback(GLFWwindow* window, int w, int h)
	{
		WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		data->height = h;
		data->width = w;

		glViewport(0, 0, w, h);
		
	}
	// === end's callback functions ===
	Window::Window(const WindowProps& props)
	{
		m_data = std::make_unique<WindowData>();
		init(props);
	}
	Window::~Window()
	{
		shutdown();
	}
	unsigned int Window::getWidth() const { return m_data->width; }
	unsigned int Window::getHeight() const { return m_data->height; }

	bool Window::isClosed() const
	{
		return glfwWindowShouldClose(m_window);
	}
	void Window::init(const WindowProps& props)
	{
		m_data->title = props.m_title;
		m_data->width = props.m_width;
		m_data->height = props.m_height;

		std::cout << "creating window " << props.m_title << " ( " << props.m_width << ", "
			<< props.m_height << ")" << std::endl;

		if (!s_GLFWInit)
		{
			int success = glfwInit();
			if (success)
			{
				s_GLFWInit = true;
				glfwSetErrorCallback(GLFWErrorCallback);

				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			}
			else
			{
				std::cerr << "failed to init glfw" << std::endl;
				return;
			}
		}
		m_window = glfwCreateWindow((int)props.m_width, (int)props.m_height, props.m_title.c_str(), nullptr, nullptr);

		if (!m_window)
		{
			std::cerr << "failder to creat glfw window" << std::endl;
			glfwTerminate();
			return;
		}

		//contectopengl
		glfwMakeContextCurrent(m_window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if(!status)
		{
			std::cerr << "failed to init glad" << std::endl;
			return;
		}
		//for callback's
		glfwSetWindowUserPointer(m_window, m_data.get());
		glfwSetWindowSizeCallback(m_window, windowResizeCallback);

		std::cout << "opengl info:\n";
		std::cout << "vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "renderer: " << glGetString(GL_RENDERER) << std::endl;
	}
	void Window::shutdown()
	{
		glfwDestroyWindow(m_window);
	}
	void Window::onUpdate()
	{
		glfwSwapBuffers(m_window);


		glfwPollEvents();
	}
}