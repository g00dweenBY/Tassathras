#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include<memory>



namespace Tassathras
{
	struct WindowData;
	// instructions for create
	struct WindowProps
	{
		std::string m_title;
		unsigned int m_width;
		unsigned int m_height;

		WindowProps(const std::string& title = "Engine", unsigned int width = 1280, unsigned int height = 720)
			: m_title(title), m_width(width), m_height(height) { }
	};

	class Window
	{
	public:
		Window(const WindowProps& proprs);
		~Window();

		void onUpdate();
		bool isClosed() const;
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		GLFWwindow* getNativeWindow() const { return m_window; }
	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();

		GLFWwindow* m_window;
		
		std::unique_ptr<WindowData> m_data;
	};
}