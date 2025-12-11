#pragma once
#include <string>

namespace Tassathras
{
	class Texture
	{
	public:
		Texture(const std::string& path);
		~Texture();

		void bind(unsigned int slot = 0) const;
		void unbind() const;

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }

	private:
		unsigned int m_rendererID;
		std::string m_filePath;
		unsigned char* m_localBuffer;
		int m_width, m_height, m_BPP;
	};
}