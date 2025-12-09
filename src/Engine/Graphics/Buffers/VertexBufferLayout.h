#pragma once
#include <glad/glad.h>
#include <vector>
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
}