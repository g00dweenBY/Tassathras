#include "Core/Window.h"
#include "Graphics/Renderer2D.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

struct Draggable
{
	glm::vec2 position;
	glm::vec2 size;
	float rotation;

	bool dragging = false;
	glm::vec2 dragOffset;


};
bool mouseInside(const Draggable& q, const glm::vec2 mouse)
{
	return mouse.x >= q.position.x - q.size.x * 0.5f &&
		mouse.x <= q.position.x + q.size.x * 0.5f &&
		mouse.y >= q.position.y - q.size.y * 0.5f &&
		mouse.y <= q.position.y + q.size.y * 0.5f;
}



int main()
{

	Tassathras::Window window("Test Renderer", 1280, 720);
	Tassathras::Renderer2D::init();

	glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);
	
	float rotation = 0.0f;
	float lastTime = glfwGetTime();

	glm::vec2 size = { 200.0f, 200.0f };

	
	Draggable drag;
	drag.position = { 300.0f, 300.0f };
	drag.size = { 200.0f, 200.0f };
	drag.rotation = 0.0f;
	




	while (!window.shouldClose())
	{
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		//rotation
		rotation += 90.0f * deltaTime;
		
		// up square
		if (glfwGetMouseButton(window.getHandle(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			size += glm::vec2(100.0f, 100.0f) * deltaTime;

		//less square
		if (glfwGetMouseButton(window.getHandle(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			size -= glm::vec2(100.0f, 100.0f) * deltaTime;


		//Drag&Drop
		double mx, my;
		glfwGetCursorPos(window.getHandle(), &mx, &my);
		my = 720 - my;
		glm::vec2 mousePos(mx, my);
		bool leftPressed = glfwGetMouseButton(window.getHandle(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

		if (leftPressed && !drag.dragging && mouseInside(drag, mousePos))
		{
			drag.dragging = true;
			drag.dragOffset = mousePos - drag.position;
		}

		if (drag.dragging)
		{
			drag.position = mousePos - drag.dragOffset;
			if (!leftPressed)
				drag.dragging = false;
		}

		if (GLFW_KEY_ESCAPE == GLFW_PRESS)
			window.shouldClose();


		window.pollEvents();
		window.clear();

		Tassathras::Renderer2D::beginScene(projection);
		Tassathras::Renderer2D::drawQuad(
			{  drag.position },		//pos
			{  drag.size },					//size
			{  0 },				//rotate
			{  1.0f, 0.0f, 0.0f, 1.0f } //color
		);
		Tassathras::Renderer2D::endScene();

		window.display();
	}

	return 0;
}