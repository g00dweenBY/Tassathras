#include "VertexBuffer.h"
#include <vector>

namespace Tassathras
{
#define GL_CALL(x) x
	
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
}