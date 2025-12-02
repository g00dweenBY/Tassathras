#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Tassathras
{
private:
	//  basic parameters
	glm::vec3 m_Position; // x, y, z ==
	float m_Zoom = 1.0f;
	float m_Rotation = 0.0f;

	// matrix
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewProjectionMatrix;

	//window parameters
	float m_ViewportWidth;
	float m_ViewportHeight;

	//isometric parameters
	float m_TileWidth = 64.0f;
	float m_TileHeight = 32.0f // m_tilewidth / 2

public:
	Camera(float viewportWidth, float viewportHeight);

	//== Control camera ==
	void setPosition(const glm::vec2& position);
	void move(const glm::vect2& delta);
	void setZoom(float zoom);
	void zoom(float delta);
	void setRotation(float rotation);

	//== isometric transformations ==
	glm::vec2 screenToWorld(const glm::vec2& screenPos) const;
	glm::vec2 WorldToScreen(const glm::vec2& worldPos) const;
	glm::vec2 screenToTile(const glm::vec2& screenPos) const;
	glm::vec2 tileToScreen(int tileX, int tileY) const;

	//parametres tile's
	void setTileDimensions(float w, float h);
	

	
	// == get == 
	float getTileWidth() const { return m_TileWidth; } 
	float getTileHeight() const { return m_TileHeight; }

	const glm::vec3& getPosition() const { return m_Position; }
	float getZoom() const { return m_Zoom; }
	float getRotation() const { return m_Rotation; }

	const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
	const glm::mat4 getProjectionMatrix() const { return m_ProjectionMatrix; }
	const glm::mat4 getViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

private:
	void recalculateViewMatrix();
	void recalculateProjectionMatrix();
	void updateViewProjectionMatrix();
}