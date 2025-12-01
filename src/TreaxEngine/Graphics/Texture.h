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

	private:
		unsigned int m_ID = 0;
		int m_Width = 0;
		int	m_Height = 0; 
		int m_Channels = 0;
	};
}