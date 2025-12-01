#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<string>



namespace TreaxEngine
{
	class Window
	{
	public:
		Window(const char* title, int width, int height);
		~Window();

		void pollEvents();
		void clear();
		void display();
		bool shouldClose() const;
		
		GLFWwindow* getHandle() const { return m_window; }

	private:
		GLFWwindow* m_window;
		int m_width;
		int	m_height;
		std::string m_title;
	};
}