#include "Game.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Log.h"
#include "Engine/Graphics/Renderers/Renderer2D.h"
#include <glad/glad.h>
#include <algorithm>
#include <cmath>

Game::Game() : Tassathras::Application() {}

void Game::onStart()
{
    TS_CORE_INFO("=== Game Editor Start ===");

    float screenWidth = 1280.0f;
    float screenHeight = 720.0f;

    m_camera = std::make_unique<Tassathras::IsometricCamera>(
        m_tileWidth, m_tileHeight, screenWidth, screenHeight, 1.0f
    );

    m_tileTexture = std::make_shared<Tassathras::Texture2D>("assets/tiles/water.png");
    m_grassTexture = std::make_shared<Tassathras::Texture2D>("assets/tiles/grass.png");
    m_dirtTexture = std::make_shared<Tassathras::Texture2D>("assets/tiles/dirt.png");
    m_sandTexture = std::make_shared<Tassathras::Texture2D>("assets/tiles/sand.png");

    Tassathras::Renderer2D::init();
    m_state = GameState::MainMenu;

    TS_CORE_INFO("Press [1-5].");
}

void Game::initMap(int size)
{
    TS_CORE_INFO("Инициализация пустого холста {}x{}", size, size);

    m_mapWidth = size;
    m_mapHeight = size;

    m_mapData.assign(m_mapWidth * m_mapHeight, Tile{ 0 });

    m_camera->setPosition({ m_mapWidth / 2.0f, m_mapHeight / 2.0f });
    m_camera->setZoom(1.0f);
    m_cameraVelocity = glm::vec2(0.0f);

    m_state = GameState::Editor;
    m_showDebugGrid = true; 
}

void Game::onUpdate(float dt)
{
    if (m_state == GameState::MainMenu)
    {
        if (Tassathras::Input::isKeyJustPressed(Tassathras::Keyboard::Key_1)) initMap(MAP_SIZE_TINY);
        if (Tassathras::Input::isKeyJustPressed(Tassathras::Keyboard::Key_2)) initMap(MAP_SIZE_SMALL);
        if (Tassathras::Input::isKeyJustPressed(Tassathras::Keyboard::Key_3)) initMap(MAP_SIZE_MEDIUM);
        if (Tassathras::Input::isKeyJustPressed(Tassathras::Keyboard::Key_4)) initMap(MAP_SIZE_LARGE);
        if (Tassathras::Input::isKeyJustPressed(Tassathras::Keyboard::Key_5)) initMap(MAP_SIZE_HUGE);
        return;
    }

    if (m_state == GameState::Editor)
    {
        glm::vec2 currentMousePos = Tassathras::Input::getMousePosition();

        if (Tassathras::Input::isMouseButtonPressed(Tassathras::Mouse::ButtonRight))
        {
            if (!m_isDragging) {
                m_isDragging = true;
                m_lastMousePos = currentMousePos;
            }
            glm::vec2 mouseDelta = currentMousePos - m_lastMousePos;
            m_cameraVelocity.x = -mouseDelta.x * m_dragSensitivity * m_camera->getZoom();
            m_cameraVelocity.y = mouseDelta.y * m_dragSensitivity * m_camera->getZoom();
            m_lastMousePos = currentMousePos;
        }
        else {
            m_isDragging = false;
        }

        glm::vec2 inputDir(0.0f);


        glm::vec2 screenInputDir(0.0f);

        float currentAccel = m_cameraAcceleration * m_camera->getZoom() * 20.0f;

        if (Tassathras::Input::isKeyPressed(Tassathras::Keyboard::W)) screenInputDir.y += 1.0f;
        if (Tassathras::Input::isKeyPressed(Tassathras::Keyboard::S)) screenInputDir.y -= 1.0f;
        if (Tassathras::Input::isKeyPressed(Tassathras::Keyboard::A)) screenInputDir.x -= 1.0f;
        if (Tassathras::Input::isKeyPressed(Tassathras::Keyboard::D)) screenInputDir.x += 1.0f;

        if (glm::length(screenInputDir) > 0.0f)
        {
            screenInputDir = glm::normalize(screenInputDir);


            glm::vec2 worldSpaceDir;
            worldSpaceDir.x = screenInputDir.x - screenInputDir.y;
            worldSpaceDir.y = -screenInputDir.x - screenInputDir.y;

            m_cameraVelocity += worldSpaceDir * currentAccel * dt;
        }
        if (glm::length(inputDir) > 0.0f)
        {
            inputDir = glm::normalize(inputDir);
            m_cameraVelocity += inputDir * currentAccel * dt;
        }

        m_cameraVelocity -= m_cameraVelocity * m_cameraFriction * dt;
        if (glm::length(m_cameraVelocity) > 0.01f) {
            m_camera->move(m_cameraVelocity * dt);
        }

        float scroll = Tassathras::Input::getScrollDelta();
        if (scroll != 0.0f) {
            float zoomSpeed = 0.15f;
            float newZoom = m_camera->getZoom() * (1.0f + scroll * zoomSpeed);
            m_camera->setZoom(std::clamp(newZoom, 0.05f, 15.0f));
        }

        if (Tassathras::Input::isKeyJustPressed(Tassathras::Keyboard::Key_1)) m_brushType = 1; // Трава
        if (Tassathras::Input::isKeyJustPressed(Tassathras::Keyboard::Key_2)) m_brushType = 2; // Песок
        if (Tassathras::Input::isKeyJustPressed(Tassathras::Keyboard::Key_3)) m_brushType = 3; // Грязь
        if (Tassathras::Input::isKeyJustPressed(Tassathras::Keyboard::Key_4)) m_brushType = 4; // Вода
        if (Tassathras::Input::isKeyJustPressed(Tassathras::Keyboard::Key_5)) m_brushType = 5; // Ластик (Пустота)

        if (Tassathras::Input::isMouseButtonPressed(Tassathras::Mouse::ButtonLeft))
        {
            glm::vec2 worldPos = m_camera->screenToWorld(currentMousePos.x, currentMousePos.y);

            int tx = static_cast<int>(std::floor(worldPos.x));
            int ty = static_cast<int>(std::floor(worldPos.y));
            if (tx >= 0 && tx < m_mapWidth && ty >= 0 && ty < m_mapHeight) {
                m_mapData[ty * m_mapWidth + tx].type = m_brushType;
            }
        }
        if (Tassathras::Input::isKeyJustPressed(Tassathras::Keyboard::G)) m_showDebugGrid = !m_showDebugGrid;
        if (Tassathras::Input::isKeyJustPressed(Tassathras::Keyboard::F1))
        {
            m_camera->setPosition({ m_mapWidth / 2.0f, m_mapHeight / 2.0f });
            m_camera->setZoom(1.0f);
            m_cameraVelocity = glm::vec2(0.0f);
        }
    }
}

void Game::onRender()
{
    glClearColor(0.05f, 0.05f, 0.07f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (m_state == GameState::MainMenu)
    {
        Tassathras::Renderer2D::beginScene(glm::mat4(1.0f));
        Tassathras::Renderer2D::endScene();
        return;
    }

    if (m_state == GameState::Editor)
    {
        Tassathras::Renderer2D::beginScene(m_camera->getViewProjectionMatrix());
        Tassathras::Renderer2D::setSortingEnabled(true);

        float zoom = m_camera->getZoom();
        auto bounds = m_camera->getVisibleBounds();

        int startX = std::max(0, (int)std::floor(bounds.minWorldX));
        int endX = std::min(m_mapWidth - 1, (int)std::ceil(bounds.maxWorldX));
        int startY = std::max(0, (int)std::floor(bounds.minWorldY));
        int endY = std::min(m_mapHeight - 1, (int)std::ceil(bounds.maxWorldY));

        for (int y = startY; y <= endY; y++) {
            for (int x = startX; x <= endX; x++) {
                int tileType = m_mapData[y * m_mapWidth + x].type;

                if (tileType == 0) continue;

                std::shared_ptr<Tassathras::Texture2D> tex;
                switch (tileType) {
                case 1:  tex = m_grassTexture; break;
                case 2:  tex = m_sandTexture;  break;
                case 3:  tex = m_dirtTexture;  break;
                case 4:  tex = m_tileTexture;  break; 
                default: tex = m_tileTexture;  break;
                }

                glm::vec2 screenPos = m_camera->worldToScreen((float)x, (float)y, 0.0f);

                Tassathras::Renderer2D::drawQuadIsometric(
                    screenPos,
                    { m_tileWidth * zoom, m_tileHeight * zoom },
                    tex,
                    -(float)(x + y) * 0.01f
                );
            }
        }

        renderCanvasBorder();

        if (m_showDebugGrid) renderDebugGrid();

        Tassathras::Renderer2D::endScene();
    }
}

void Game::renderCanvasBorder()
{
    glm::vec2 p0 = m_camera->worldToScreen(0.0f, 0.0f, 0.1f);
    glm::vec2 p1 = m_camera->worldToScreen((float)m_mapWidth, 0.0f, 0.1f);
    glm::vec2 p2 = m_camera->worldToScreen((float)m_mapWidth, (float)m_mapHeight, 0.1f);
    glm::vec2 p3 = m_camera->worldToScreen(0.0f, (float)m_mapHeight, 0.1f);

    glm::vec4 color = { 0.0f, 0.0f, 0.0f, 0.1f }; 
    Tassathras::Renderer2D::drawLine(p0, p1, color);
    Tassathras::Renderer2D::drawLine(p1, p2, color);
    Tassathras::Renderer2D::drawLine(p2, p3, color);
    Tassathras::Renderer2D::drawLine(p3, p0, color);
}

void Game::renderDebugGrid()
{
    glm::vec4 gridColor = { 0.1f, 0.1f, 0.1f, 1.0f };

    for (int x = 0; x <= m_mapWidth; x++) {
        glm::vec2 start = m_camera->worldToScreen((float)x, 0.0f, 0.05f);
        glm::vec2 end = m_camera->worldToScreen((float)x, (float)m_mapHeight, 0.05f);
        Tassathras::Renderer2D::drawLine(start, end, gridColor);
    }

    for (int y = 0; y <= m_mapHeight; y++) {
        glm::vec2 start = m_camera->worldToScreen(0.0f, (float)y, 0.05f);
        glm::vec2 end = m_camera->worldToScreen((float)m_mapWidth, (float)y, 0.05f);
        Tassathras::Renderer2D::drawLine(start, end, gridColor);
    }
}