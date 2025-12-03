#include <iostream>
#include "Core/Scene.h"

/*
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

*/



int main()
{
	std::cout << "blyad\n";
	Tassathras::Scene app;
	app.run();

}