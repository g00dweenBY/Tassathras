#include "VertexArray.h"
#include <iostream>
#include <stdexcept>

namespace Tassathras
{
#define GL_CALL(x) x

	//======================================================================
	// VertexBufferLayout Implementations
	//======================================================================
	template<>
	void VertexBufferLayout::push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}
	
	template<>
	void VertexBufferLayout::push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void VertexBufferLayout::push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
		m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}
	//======================================================================
	// VertexBuffer implementation (VBO)
	//======================================================================
	VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	{
		GL_CALL(glGenBuffers(1, &m_rendererID));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}
	VertexBuffer::~VertexBuffer()
	{
		GL_CALL(glDeleteBuffers(1, &m_rendererID));
	}

	void VertexBuffer::bind() const
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
	}
	void VertexBuffer::unbind() const
	{
		GL_CALL(glBindBuffer, 0);
	}
	
	void VertexBuffer::setData(const void* data, unsigned int size)
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
		GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}
	//======================================================================
	// Indexbuffer (ebo/ibo) implementations
	//======================================================================

	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_count(count)
	{
		GL_CALL(glGenBuffers(1, &m_rendererID));
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
		
		GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	IndexBuffer::~IndexBuffer()
	{
		GL_CALL(glDeleteBuffers(1, &m_rendererID));
	}

	void IndexBuffer::bind() const
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
	}
	
	void IndexBuffer::unbind() const
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	//======================================================================
	// vertexarray (vao)
	//======================================================================

	VertexArray::VertexArray()
	{
		GL_CALL(glGenVertexArrays(1, &m_rendererID));
	}

	VertexArray::~VertexArray()
	{
		GL_CALL(glDeleteVertexArrays(1, &m_rendererID));
	}


	void VertexArray::bind() const
	{
		GL_CALL(glBindVertexArray(m_rendererID));
	}
	void VertexArray::unbind() const
	{
		GL_CALL(glBindVertexArray(0));
	}

	void VertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		GL_CALL(glBindVertexArray(m_rendererID));

		vertexBuffer->bind();

		const auto& layout = vertexBuffer->getLayout();

		unsigned int offset = 0;

		for (unsigned int i = 0; i < layout.getElements().size(); i++)
		{
			const auto& element = layout.getElements()[i];

			GL_CALL(glEnableVertexAttribArray(i));

			GL_CALL(glVertexAttribPointer(
				i,
				element.count,
				element.type,
				element.normalized,
				layout.getStride(),
				(const void*)(intptr_t)offset
			));
			offset += element.count * VertexBufferElement::getSizeOfType(element.type);
		}
		m_vertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		GL_CALL(glBindVertexArray(m_rendererID));
		indexBuffer->bind();
		m_indexBuffers = indexBuffer;
	}

}