#pragma once


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Tassathras
{
	class Shader
	{
	public:
		Shader(const std::string& filepath);
		~Shader();

		void bind() const;
		void unbind() const;

		void setInt(const std::string& name, int value);
		void setFloat(const std::string& name, float value);
		void setFloat4(const std::string& name, float f0, float f1, float f2, float f3);
		void setMat4(const std::string& name, const glm::mat4& matrix);

	private:
		unsigned int m_rendererID;
		std::string m_name;

		std::unordered_map<std::string, int> m_uniformLocationCache;

		std::string readFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> preProcess(const std::string& source);
		void compile(const std::unordered_map<GLenum, std::string>& shaderSource);

		int getUniformLocation(const std::string& name);

	};
}