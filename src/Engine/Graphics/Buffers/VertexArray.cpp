#include "VertexArray.h"

namespace Tassathras
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_rendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_rendererID);
	}


	void VertexArray::bind() const
	{
		glBindVertexArray(m_rendererID);
	}
	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_rendererID);

		vertexBuffer->bind();

		const auto& layout = vertexBuffer->getLayout();

		unsigned int offset = 0;

		for (unsigned int i = 0; i < layout.getElements().size(); i++)
		{
			const auto& element = layout.getElements()[i];

			glEnableVertexAttribArray(i);

			glVertexAttribPointer(
				i,
				element.count,
				element.type,
				element.normalized,
				layout.getStride(),
				(const void*)(intptr_t)offset
			);
			offset += element.count * VertexBufferElement::getSizeOfType(element.type);
		}
		m_vertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_rendererID);
		indexBuffer->bind();
		m_indexBuffers = indexBuffer;
	}
}