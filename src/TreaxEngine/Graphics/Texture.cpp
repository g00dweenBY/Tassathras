#include <glad/glad.h>
#include <iostream>
#include "Texture.h"
#include<stb_image.h>

namespace Tassathras
{
	Texture::Texture(const std::string& path) : m_ID(0), m_Width(0), m_Height(0), m_Channels(0)
	{
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);
		if (!data)
		{
			std::cerr << "Feiled to load texture: " << path << std::endl;
			return;
		}
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum format = m_Channels == 4 ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_ID);
	}
	void Texture::bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
	void Texture::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}