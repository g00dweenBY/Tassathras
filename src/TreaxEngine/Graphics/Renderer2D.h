#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<memory>
#include<cstdint>

namespace Tassathras
{
	class Shader;
	class Camera;

	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void beginScene(const Camera& camera);
		static void endScene();
		// general method drawing quads
		static void drawQuad(
			const glm::vec3& pos,
			const glm::vec2& size,
			float rotation = 0.0f,
			const glm::vec4& color = glm::vec4(1.0f)
		);
		
		// alternative method with done matrix transformation
		static void drawQuad(
			const glm::mat4& transform,
			const glm::vec4& color = glm::vec4(1.0f)
		);
		//secondary method 
		static void drawRotateQuad(
			const glm::vec2& pos,
			const glm::vec2& size,
			float rotationDegrees,
			const glm::vec4& color = glm::vec4(1.0f)
		);
		// Statistic
		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;

			uint32_t getTotalVertexcount() const { return quadCount * 4; }
			uint32_t getTotalIndexCount() const { return quadCount * 6; }
		};

		static void resetStatistics();
		static Statistics getStatistics();

	private:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec4 color;
		};

		//consts renderer
		static constexpr uint32_t MAX_QUADS = 20000;
		static constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4;
		static constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;

		//secondary methods
		static void flushAndReset();
		static void startBatch();
		static void flush();

	private:
		//graphic resources
		static uint32_t m_quadVA;
		static uint32_t m_quadVB;
		static uint32_t m_quadIB;

		//batching
		static std::unique_ptr<Vertex[]> m_vertexBufferBase;
		static Vertex* m_vertexBufferPtr;
		static uint32_t m_indexCount;

		//shaders and matrix

		static std::unique_ptr<Shader> m_Shader;
		static glm::mat4 m_viewProjectionMatrix;

		//statistics
		static Statistics m_statistics;

		static bool m_initialized;

	};
}