#pragma once
#include <glad/glad.h>
#include <vector>
#include <memory>

namespace Tassathras
{
	struct VertexBufferElement
	{
		GLenum type;
		unsigned int count;
		bool normalized;

		static unsigned int getSizeOfType(GLenum type)
		{
			switch (type)
			{
				case GL_FLOAT:		   return 4;
				case GL_UNSIGNED_INT:  return 4;
				case GL_UNSIGNED_BYTE: return 1;
			}
			return 0;
		}
	};

	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> m_elements;
		unsigned int m_stride = 0;

	public:
		template<typename T>
		void push(unsigned int count) = delete;

		inline const std::vector<VertexBufferElement>& getElements() const { return m_elements; }
		inline unsigned int getStride() const { return m_stride; }

	};

	template<>
	void VertexBufferLayout::push<float>(unsigned int count);

	template<>
	void VertexBufferLayout::push<unsigned int>(unsigned int count);

	//vbo
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
	//ebo
	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		inline unsigned int getCount() const { return m_count; }

	private:
		unsigned int m_rendererID;
		unsigned int m_count;
	};

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