#pragma once
#include <glm/glm.hpp>
#include <array>

namespace Tassathras
{
	class Input
	{
	public:
		static bool isKeyPressed(int keycode);
		static bool isMouseButtonPressed(int button);
		static glm::vec2 getMousePosition();

		static void setKeyPressed(int keycode, bool isPressed);
		static void setMouseButtonPressed(int button, bool isPressed);
		static void setMousePosition(float x, float y);

	private:
		static constexpr size_t MAX_KEYS = 512;
		static constexpr size_t MAX_MOUSE_BUTTONS = 8;

		static std::array<bool, MAX_KEYS> m_keys;
		static std::array<bool, MAX_MOUSE_BUTTONS> m_mouseButtons;
		static glm::vec2 m_mousePos;

	private:
		Input() = delete;
		~Input() = delete;
	};
}