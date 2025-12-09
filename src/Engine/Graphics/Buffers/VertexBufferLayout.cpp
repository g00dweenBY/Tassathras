#include "VertexBufferLayout.h"

namespace Tassathras
{
    template<>
    void VertexBufferLayout::push<float>(unsigned int count) {
        m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
        m_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
    }

    template<>
    void VertexBufferLayout::push<unsigned int>(unsigned int count) {
        m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void VertexBufferLayout::push<unsigned char>(unsigned int count) {
        // Для цвета мы используем GL_TRUE, чтобы OpenGL сам преобразовал [0, 255] в [0.0f, 1.0f]
        m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
    }
}