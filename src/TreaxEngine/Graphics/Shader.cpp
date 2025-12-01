#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"

namespace Tassathras
{
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::string vertexSrc = loadShaderSource(vertexPath);
		std::string fragmentSrc = loadShaderSource(fragmentPath);

		if (vertexSrc.empty() || fragmentSrc.empty())
		{
			std::cerr << "error one of the shader files is empty" << std::endl;
		}

		m_ID = createShader(vertexSrc, fragmentSrc);
	}

	Shader::~Shader()
	{
		if(m_ID)
			glDeleteProgram(m_ID);
	}

	void Shader::bind() const { glUseProgram(m_ID); }
	void Shader::unbind() const { glUseProgram(0); }

	void Shader::setMat4(const std::string &name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
	}
	void Shader::setVec4(const std::string& name, const glm::vec4& vec)
	{
		glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vec[0]);
	}

	
	std::string Shader::loadShaderSource(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
		{
			std::cerr << "failed to open shader file: " << filepath << std::endl;
			return "";
		}
		std::stringstream ss;
		ss << file.rdbuf();
		return ss.str();
	}

	unsigned int Shader::compileShader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int logLen = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
			std::vector<char> infoLog(logLen);
			glGetShaderInfoLog(id, logLen, nullptr, infoLog.data());
			std::cerr << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
				<< " shader compile failed:\n" << infoLog.data() << std::endl;
		}

		return id;
	}

	unsigned int Shader::createShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexSrc);
		unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);

		int success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			int logLen = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
			std::vector<char> infoLog(logLen);
			glGetProgramInfoLog(program, logLen, nullptr, infoLog.data());
			std::cerr << "shader linking failed:\n" << infoLog.data() << std::endl;
		}
		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}
}