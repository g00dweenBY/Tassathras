#pragma once
#include<glad/glad.h>
#include <string>
#include <glm/glm.hpp>


namespace Tassathras
{
	class Shader
	{
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void bind() const;
		void unbind() const;

		//download uniform-variables
		void setMat4(const std::string& name, const glm::mat4& matrix);
		void setVec4(const std::string& name, const glm::vec4& vec);

	private:
		unsigned int m_ID;

		std::string loadShaderSource(const std::string& filepath);
		unsigned int compileShader(unsigned int type, const std::string& source);
		unsigned int createShader(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}