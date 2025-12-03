#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Application.h"
#include "Graphics/Camera.h"
#include "Graphics/Renderer2D.h"

namespace Tassathras
{
	
	struct MovingQuad
	{
		glm::vec3 pos;
		glm::vec2 size;
		glm::vec4 color;
		float rotation;
		glm::vec2 velocity;
	};

	class Scene : public Application
	{
	private:
		Camera m_camera;
		std::vector<MovingQuad> m_quads;

		//controller camera
		glm::vec2 m_cameraPosition = { 0.0f, 0.0f };
		float m_cameraZoom = 1.0f;
		float m_cameraRotation = 0.0f;

		//variables for testing;
		bool m_showGrid = true;
		bool m_animateQuads = true;
		int m_quadCount = 100;
		float m_time = 0.0f;

		void createTestQuads();
		void updateCamera(float deltaTime);
		void drawGrid();
		void drawStatistics();

	public:
		Scene();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onEvent() override;
	};
}