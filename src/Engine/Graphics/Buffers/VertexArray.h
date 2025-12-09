#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <memory>
#include <vector>
namespace Tassathras
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void bind() const;
		void unbind() const;

		void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);


		inline const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const { return m_vertexBuffers; }
		inline const std::shared_ptr<IndexBuffer>& getIndexBuffer() const { return m_indexBuffers; }

	private:
		unsigned int m_rendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffers;
	};
}