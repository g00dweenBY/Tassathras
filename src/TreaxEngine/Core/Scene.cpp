#include <random>
#include <iostream>
#include "Scene.h"

namespace Tassathras
{
	Scene::Scene()
	{
		Renderer2D::init();

		m_camera.setPosition({ 0.0f, 0.0f });
		m_camera.setRotation(0.0f);
		m_camera.setZoom(1.0f);

		createTestQuads();
	}
	
	void Scene::createTestQuads()
	{
		m_quads.clear();

		std::random_device rd;
		std::mt19937 gen(rd());

		std::uniform_real_distribution<> posDist(-8.0, 8.0);
		std::uniform_real_distribution<> sizeDist(0.3, 1.0);

		std::uniform_real_distribution<> colorDist(0.0, 1.0);
		std::uniform_real_distribution<> velDist(-2.0, 2.0);

		for (int i = 0; i < m_quadCount; i++)
		{
			MovingQuad quad;
			//position
			quad.pos = {
				static_cast<float>(posDist(gen)),
				static_cast<float>(posDist(gen)),
				0.0f
			};
			//size
			quad.size = {
				static_cast<float>(sizeDist(gen)),
				static_cast<float>(sizeDist(gen))
			};
			//color
			quad.color = {
				static_cast<float>(colorDist(gen)), //R
				static_cast<float>(colorDist(gen)), //G
				static_cast<float>(colorDist(gen)), //B
				0.8f								//A
			};

			quad.rotation = 0.0f;

			quad.velocity = {
				static_cast<float>(velDist(gen)),
				static_cast<float>(velDist(gen))
			};

			m_quads.push_back(quad);

		}

		std::cout << "created " << m_quads.size() << " testing squares" << std::endl;
	}
	void Scene::updateCamera(float deltaTime)
	{
		GLFWwindow* window = getWindow();
		if (!window) return;
		
		float cameraSpeed = 5.0f * m_cameraZoom;
		//camera movement
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_cameraPosition.y += cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_cameraPosition.y -= cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_cameraPosition.x -= cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_cameraPosition.x += cameraSpeed * deltaTime;

		//zoom
		float zoomSpeed = 2.0f;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			m_cameraZoom *= 1.0f + zoomSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			m_cameraZoom /= 1.0f + zoomSpeed * deltaTime;

		//rotation
		float rotationSpeed = 45.0f;
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			m_cameraRotation += rotationSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			m_cameraRotation -= rotationSpeed * deltaTime;

		m_cameraZoom = glm::clamp(m_cameraZoom, 0.1f, 10.0f);

		int w, h;
		glfwGetWindowSize(window, &w, &h);
		float aspectRatio = static_cast<float>(w) / h;
		
		m_camera.setPosition(m_cameraPosition);
		m_camera.setRotation(m_cameraRotation);
		m_camera.setZoom(m_cameraZoom);
	}
	void Scene::onUpdate(float deltaTime)
	{
		m_time += deltaTime;
		updateCamera(deltaTime);

		if (m_animateQuads)
		{
			for (auto& quad : m_quads)
			{
				//movement
				quad.pos.x += quad.velocity.x * deltaTime;
				quad.pos.y += quad.velocity.y * deltaTime;

				const float boundary = 10.0f;
				if (quad.pos.x < -boundary || quad.pos.x > boundary)
					quad.velocity.x *= -1.0f;
				if (quad.pos.y < -boundary || quad.pos.y > boundary)
					quad.velocity.y *= -1.0f;
				//rotation
				quad.rotation += deltaTime * 45.0f; // 45 degrees per second
			}
		}
	}

	void Scene::onRender()
	{
		Renderer2D::beginScene(m_camera);
		if (m_showGrid)
			drawGrid();

		for (const auto& quad : m_quads)
		{
			Renderer2D::drawQuad(
				quad.pos,
				quad.size,
				glm::radians(quad.rotation),
				quad.color
			);
		}
		Renderer2D::drawQuad(
			{ 0.0f, 0.0f, 0.0f },
			{ 2.0f, 2.0f },
			glm::radians(m_time * 30.0f),
			{ 1.0f, 1.0f, 1.0f, 0.3f }
		);

		Renderer2D::endScene();
		drawStatistics();
	}

	void Scene::drawGrid()
	{
		//simple girid implementation
		const float gridSize = 20.0f;
		const float gridStep = 1.0f;
		const float axisLineWidth = 0.02f;
		const float gridLineWidth = 0.01f;
		//colors
		const glm::vec4 gridColor = { 0.5f, 0.5f, 0.5f, 0.3f };
		const glm::vec4 axisColorX = { 1.0f, 0.3f, 0.3f, 0.8f };
		const glm::vec4 axisColorY = { 0.3f, 0.5f, 0.5f, 0.3f };

		// X
		Renderer2D::drawQuad(
			{ 0.0f, 0.0f, -0.1f },
			{ gridSize * 2.0f, axisLineWidth },
			0.0f,
			axisColorX
		);
		//Y
		Renderer2D::drawQuad(
			{ 0.0f, 0.0f, -0.1f },
			{ axisLineWidth, gridSize * 2.0f },
			0.0f,
			axisColorY
		);

		for (float y = -gridSize; y <= gridSize; y += gridStep)
		{
			if (std::abs(y) < 0.01f) continue;

			Renderer2D::drawQuad(
				{ 0.0f, y, -0.2f },
				{ gridSize * 2.0f, gridLineWidth },
				0.0f,
				gridColor
			);
		}

		for (int i = -10; i < 10; ++i)
		{
			if (i == 0) continue;

			Renderer2D::drawQuad(
				{ static_cast<float>(i), 0.0f, -0.1f },
				{ 0.05f, 0.05f },
				0.0f,
				axisColorX
			);

			
			Renderer2D::drawQuad(
				{ 0.0f, static_cast<float>(i), -0.1f },
				{ 0.05f, 0.05f },
				0.0f,
				axisColorY
			);
		}
		
	}

	void Scene::drawStatistics()
	{
		static float timer = 0.0f;
		static int frameCount = 0;

		timer += 0.016f;
		frameCount++;

		if (timer >= 1.0f)
		{
			auto stats = Renderer2D::getStatistics();
			//clear console(windows)
            #ifdef _WIN32
			system("cls");
			#else
			system("clear");
			#endif

			std::cout << "== Renderer Statistics ==\n";
			std::cout << "Frame: " << frameCount << '\n';
			std::cout << "FPS" << static_cast<int>(frameCount / timer) << '\n';
			std::cout << "Quads: " << stats.quadCount << '\n';
			std::cout << "Draw calls: " << stats.drawCalls << '\n';
			std::cout << "Vertices: " << stats.getTotalVertexcount() << '\n';
			std::cout << "indices: " << stats.getTotalIndexCount() << '\n';
			std::cout << "\n=== CAMERA INFO ===\n";
			std::cout << "Position: (" << m_cameraPosition.x << ", " << m_cameraPosition.y << ")\n";
			std::cout << "Zoom: " << m_cameraZoom << "\n";
			std::cout << "Rotation: " << m_cameraRotation << "°\n";
			std::cout << "\n=== SCENE INFO ===\n";
			std::cout << "Total Quaqds: " << m_quads.size() << "\n";
			std::cout << "Grid: " << (m_showGrid ? "ON" : "OFF") << "\n";
			std::cout << "Animation: " << (m_animateQuads ? "ON" : "OFF") << "\n";
			std::cout << "\n=== CONTROLS ===\n";
			std::cout << "WASD - Move Camera\n";
			std::cout << "Q/E - Zoom in/out\n";
			std::cout << "Z/X - Rotate camera\n";
			std::cout << "G - Toggle grid\n";
			std::cout << "SPACE - Toggle animation\n";
			std::cout << "R - Reset scene\n";
			std::cout << "ESC - Exin\n";
			std::cout << "=====================================\n";

			timer = 0.0f;
			frameCount = 0;

			Renderer2D::resetStatistics();
		}
	}
	void Scene::onEvent()
	{
		GLFWwindow* window = getWindow();
		if (!window) return;
		// флаги для обработки однократных нажатий
		static bool spacePressed = false;
		static bool gPressed = false;
		static bool rPressed = false;
		static bool f1Pressed = false;
		
		//SPACE - toggle animation
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed)
		{
			m_animateQuads = !m_animateQuads;
			std::cout << "Animation: " << (m_animateQuads ? "ENABLED" : "DISABLED") << std::endl;
			spacePressed;
		}
		else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
			spacePressed = false;

		//G - toggle grid
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !gPressed)
		{
			m_showGrid = !m_showGrid;
			std::cout << "Grid: " << (m_showGrid ? "SHOWN" : "HIDDEN") << std::endl;
			gPressed = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE)
			gPressed = false;

		//R - restart scene(create new quad)
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !rPressed)
		{
			createTestQuads();
			std::cout << "Scene reset" << std::endl;
			rPressed = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
			rPressed = false;

		//F - reset camera to default position
		if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && !f1Pressed)
		{
			m_cameraPosition = { 0.0f, 0.0f };
			m_cameraZoom = 1.0f;
			m_cameraRotation = 0.0f;
			std::cout << "Camer reset to default position" << std::endl;
			f1Pressed = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE)
			f1Pressed = false;

		static bool key1Pressed = false;
		static bool key2Pressed = false;
		static bool key3Pressed = false;

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !key1Pressed)
		{
			m_quadCount = 10;
			createTestQuads();
			key1Pressed = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE)
			key1Pressed = false;

		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !key2Pressed)
		{
			m_quadCount = 50;
			createTestQuads();
			key2Pressed = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE)
			key2Pressed = false;

		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !key3Pressed)
		{
			m_quadCount = 10;
			createTestQuads();
			key3Pressed = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE)
			key3Pressed = false;


	}

}