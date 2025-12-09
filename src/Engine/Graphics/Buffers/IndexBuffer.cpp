#include "IndexBuffer.h"
#include <glad/glad.h>
namespace Tassathras
{
#define GL_CALL(x) x

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
}