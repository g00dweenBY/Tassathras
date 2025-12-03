#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Tassathras
{
	class Camera
	{
	private:
		//  basic parameters
		glm::vec3 m_position; // x, y, z ==
		float m_zoom = 1.0f;
		float m_rotation = 0.0f;
		//float m_aspectRatio = 16.0f / 9.0f;

		// matrix
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewProjectionMatrix;

		//window parameters
		float m_viewportWidth;
		float m_viewportHeight;

		//isometric parameters
		float m_tileWidth = 64.0f;
		float m_tileHeight = 32.0f; // m_tilewidth / 2

	public:
		Camera(float viewportWidth, float viewportHeight);
		Camera();
		//== Control camera ==
		void setPosition(const glm::vec2& position);
		void move(const glm::vec2& delta);
		void setZoom(float zoom);
		void zoom(float delta);
		void setRotation(float rotation);
		//void setAspectRatio(float aspectRatio);

		//== isometric transformations ==
		glm::vec2 screenToWorld(const glm::vec2& screenPos) const;
		glm::vec2 WorldToScreen(const glm::vec2& worldPos) const;
		glm::vec2 screenToTile(const glm::vec2& screenPos) const;
		glm::vec2 tileToScreen(int tileX, int tileY) const;

		//parametres tile's
		void setTileDimensions(float w, float h);



		// == get == 
		float getTileWidth() const { return m_tileWidth; }
		float getTileHeight() const { return m_tileHeight; }

		const glm::vec3& getPosition() const { return m_position; }
		float getZoom() const { return m_zoom; }
		float getRotation() const { return m_rotation; }

		const glm::mat4& getViewMatrix() const { return m_viewMatrix; }
		const glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; }
		const glm::mat4 getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

	private:
		void recalculateViewMatrix();
		void recalculateProjectionMatrix();
		void updateViewProjectionMatrix();
	};
}