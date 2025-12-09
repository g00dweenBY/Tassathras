#pragma once
#include "VertexBufferLayout.h"
#include <glad/glad.h>
#include <vector>
namespace Tassathras
{


	class VertexBuffer
	{
	public:
		VertexBuffer(unsigned int size); //fix size vbo
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void bind() const;
		void unbind() const;

		void setData(const void* data, unsigned int size);

		void setLayout(const VertexBufferLayout& layout) { m_layout = layout; }
		inline const VertexBufferLayout& getLayout() const { return m_layout; }

	private:
		unsigned int m_rendererID;

		VertexBufferLayout m_layout;
	};
}