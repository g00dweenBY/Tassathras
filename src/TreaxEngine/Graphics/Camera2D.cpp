#include "Camera2D.h"

namespace Tassathras
{
	Camera::Camera(float viewportWidth, float viewportHeight) :
		m_viewportWidth(viewportWidth),
		m_viewportHeight(viewportHeight)
	{
		m_position = glm::vec3(0.0f, 0.0f, 100.0f); // z = 100 

		recalculateProjectionMatrix();
		recalculateViewMatrix();
		updateViewProjectionMatrix();
	}

	// ========= camera update =============
	void Camera::recalculateProjectionMatrix()
	{
		float left = -m_viewportWidth * 0.5f / m_zoom;
		float right = m_viewportWidth * 0.5f / m_zoom;
		float bottom = -m_viewportWidth * 0.5f / m_zoom;
		float top = m_viewportWidth * 0.5f / m_zoom;

		m_projectionMatrix = glm::ortho(
			left,
			right,
			bottom,
			top,
			0.1f,
			1000.0f
		);
	}
	void Camera::recalculateViewMatrix()
	{
		//камера смотрит вниз по оси Z
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position);
		transform = glm::rotate(transform, glm::radians(m_rotation), glm::vec3(0, 0, 1));

		m_viewMatrix = glm::inverse(transform);
	}
	
	void Camera::updateViewProjectionMatrix()
	{
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	// ================== Controlling ===================
	void Camera::setPosition(const glm::vec2& position)
	{
		m_position.x = position.x;
		m_position.y = position.y;
		
		recalculateViewMatrix();
		updateViewProjectionMatrix();
	}
	void Camera::move(const glm::vec2& delta)
	{
		m_position.x += delta.x / m_zoom;
		m_position.y += delta.y / m_zoom;

		recalculateViewMatrix();
		updateViewProjectionMatrix();
	}

	void Camera::setZoom(float zoom)
	{
		m_zoom = glm::clamp(zoom, 0.1f, 10.0f);
		
		recalculateProjectionMatrix();
		updateViewProjectionMatrix();
	}

	void Camera::zoom(float delta)
	{
		setZoom(m_zoom + delta * m_zoom * 0.1f);
	}

	void Camera::setRotation(float rotation)
	{
		m_rotation = rotation;
		
		recalculateViewMatrix();
		updateViewProjectionMatrix();
	}

	void Camera::setTileDimensions(float w, float h)
	{
		m_tileWidth = w;
		m_tileHeight = h;
	}
	// ================== Controlling =================== /

	glm::vec2 Camera::screenToWorld(const glm::vec2& screenPos) const
	{
		//convert screen coords в NDC
		float ndcX = (2.0f * screenPos.x) / m_viewportWidth - 1.0f;
		float ndcY = 1.0f - (2.0f * screenPos.y) / m_viewportHeight;
		//convert NDC to world coords
		glm::vec4 worldPos = glm::inverse(m_viewProjectionMatrix) * glm::vec4(ndcX, ndcY, 0.0f, 1.0f);

		return glm::vec2(worldPos.x, worldPos.y);
	}

	glm::vec2 Camera::WorldToScreen(const glm::vec2& worldPos) const
	{
		// world -> NDC
		glm::vec4 ndc = m_viewProjectionMatrix * glm::vec4(worldPos, 0.0f, 1.0f);
		// NDC -> screen
		float screenX = (ndc.x + 1.0f) * m_viewportWidth * 0.5f;
		float screenY = (1.0 - ndc.y) * m_viewportHeight * 0.5f;

		return glm::vec2(screenX, screenY);
	}

	glm::vec2 Camera::screenToTile(const glm::vec2& screenPos) const
	{
	    // screnn -> world
		glm::vec2 worldPos = screenToWorld(screenPos);

		//world -> isometric coords worldPos already in the isometric system
		// isometric - tiles coords
		float tileX = (worldPos.x / (m_tileWidth * 0.5f) +
			           worldPos.y / (m_tileHeight * 0.5f)) * 0.5f;

		float tileY = (worldPos.y / (m_tileHeight * 0.5f) -
					   worldPos.x / (m_tileWidth * 0.5f)) * 0.5f;

		return glm::vec2(tileX, tileY);
	}
	
	glm::vec2 Camera::tileToScreen(int tileX, int tileY) const
	{
		// tiles-> isometric coords
		float isoX = (tileX - tileY) * m_tileWidth * 0.5f;
		float isoY = (tileX + tileY) * m_tileHeight * 0.5f;
		// isometric -> screen
		return WorldToScreen(glm::vec2(isoX, isoY));
	}
}