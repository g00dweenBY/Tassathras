#pragma once
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include<memory>

namespace Tassathras
{
	class Application
	{
	private:
		void init();
		void shutDown();
	private:
		GLFWwindow* m_window = nullptr;
		int m_width = 1280;
		int m_height = 720;
		const char* m_title = "Tassathras engine";
		bool m_running = true;
		float m_lastFrameTime = 0.0f;

	public:
		Application();
		virtual ~Application();

		void run();

		virtual void onUpdate(float deltaTime) {}
		virtual void onRender() {}
		virtual void onEvent() {}

		GLFWwindow* getWindow() const { return m_window; };
	};
}