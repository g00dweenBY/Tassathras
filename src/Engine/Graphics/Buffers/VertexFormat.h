#pragma once
#include "ShaderDataType.h"
#include <vector>
#include <string>

namespace Tassathras
{
	struct VertexElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		VertexElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name)
			, Type(type)
			, Size(ShaderDataTypeSize(type))
			, Offset(0)
			, Normalized(normalized) 
		{}

		uint32_t GetComponentCount() const {
			switch (Type) {
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Int:     return 1;
			}
			return 0;
		}
	};

	class VertexFormat
	{
	public:
		VertexFormat() {}
		VertexFormat(const std::initializer_list<VertexElement>& elements) : m_elements(elements)
		{
			uint32_t offset = 0;
			m_stride = 0;
			for (auto& element : m_elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_stride += element.Size;
			}
		}
		uint32_t getStride() const { return m_stride; }

		const std::vector<VertexElement>& getElements() const { return m_elements; }

		std::vector<VertexElement>::iterator begin() { return m_elements.begin(); }
		std::vector<VertexElement>::iterator end() { return m_elements.end(); }
		std::vector<VertexElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<VertexElement>::const_iterator end() const { return m_elements.end(); }

	private:
		std::vector<VertexElement> m_elements;
		uint32_t m_stride;
	};
}