#include "Input.h"
#include <GLFW/glfw3.h>

namespace Tassathras
{
	std::array<bool, Input::MAX_KEYS> Input::m_keys = {};
	std::array<bool, Input::MAX_MOUSE_BUTTONS> Input::m_mouseButtons = {};
	glm::vec2 Input::m_mousePos = { 0.0f, 0.0f };

	bool Input::isKeyPressed(int keycode)
	{
		if (keycode >= 0 && keycode < MAX_KEYS)
			return m_keys[keycode];
		return false;
	}


	bool Input::isMouseButtonPressed(int button)
	{
		if (button >= 0 && button < MAX_MOUSE_BUTTONS)
			return m_mouseButtons[button];
		return false;
	}

	glm::vec2 Input::getMousePosition()
	{
		return m_mousePos;
	}

	void Input::setKeyPressed(int keycode, bool isPressed)
	{
		if (keycode >= 0 && keycode < MAX_KEYS)
			m_keys[keycode] = isPressed;
	}
	void Input::setMouseButtonPressed(int button, bool isPressed)
	{
		if (button >= 0 && button < MAX_MOUSE_BUTTONS)
			m_mouseButtons[button] = isPressed;
	}

	void Input::setMousePosition(float x, float y)
	{
		m_mousePos.x = x;
		m_mousePos.y = y;
	}
}