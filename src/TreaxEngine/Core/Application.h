#pragma once

#include "Window.h"
#include <functional>

namespace TreaxEngine
{
	class Application
	{
	public:
		Application();
		~Application();

		void createWindow(const char* title, int width, int height);
		void run(const std::function<void()>& updateFunc);

	private:

		Window* m_window;
		bool m_running;
	};
}