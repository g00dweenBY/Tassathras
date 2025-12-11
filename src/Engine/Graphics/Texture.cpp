#include "Texture.h"
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>

namespace Tassathras
{
	Texture::Texture(const std::string& path)
		: m_rendererID(0), m_filePath(path), m_localBuffer(nullptr),
		  m_width(0), m_height(0), m_BPP(0)
	{
		stbi_set_flip_vertically_on_load(1);

		m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 4);

		if (m_localBuffer)
		{
			glGenTextures(1, &m_rendererID);
			glBindTexture(GL_TEXTURE_2D, m_rendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);

			glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(m_localBuffer);
		}
		else
			std::cerr << "failed to load texture: " << path << std::endl;
	}
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_rendererID);
	}

	void Texture::bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_rendererID);
	}

	void Texture::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}