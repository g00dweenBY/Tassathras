#pragma once
#include "Window.h"
#include "Graphics/Shader.h"
#include "Graphics/Buffers/VertexArray.h"

#include <memory>
#include <iostream>
#include <glm/glm.hpp>



namespace Tassathras
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		static Application& get() { return *s_instance; }
		Tassathras::Window& getWindow() { return *m_window; }

	public:
		Application(const Application& other) = delete;
		Application& operator=(const Application& other) = delete;

		Application(Application&& other) noexcept = default;
		Application& operator=(Application&& other) noexcept = default;


	private:
		void init();
		void shutdown();

		std::unique_ptr<Tassathras::Window> m_window;
		bool m_running = true;
		static Application* s_instance;

		std::shared_ptr<Tassathras::Shader> m_shader;
		std::shared_ptr<Tassathras::VertexArray> m_vertexArray;
	};
}
