#include "Shader.h"
#include <glm/gtc/type_ptr.hpp> // for value_ptr
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace Tassathras
{
	static GLenum shaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
		return 0;
	}

	Shader::Shader(const std::string& filepath) : m_rendererID(0), m_name(filepath)
	{
		std::string source = readFile(filepath);
		auto shaderSource = preProcess(source);
		compile(shaderSource);
	}
	Shader::~Shader()
	{
		glDeleteProgram(m_rendererID);
	}

	std::string Shader::readFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			result.resize(size);
			in.seekg(0, std::ios::beg);
			in.read(&result[0], size);
		}
		else
			std::cerr << "could not open file: " << filepath << std::endl;
		return result;
	}

	std::unordered_map<GLenum, std::string> Shader::preProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#shader";
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			size_t begin = pos + strlen(typeToken) + 1;

			std::string type = source.substr(begin, eol - begin);

			size_t nextPos = source.find(typeToken, eol);
			shaderSources[shaderTypeFromString(type)] = source.substr(eol, nextPos - eol);
			pos = nextPos;
		}
		return shaderSources;
	}
	void Shader::compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		unsigned int program = glCreateProgram();
		std::vector<unsigned int> glShaderIDs;

		for (auto& [type, source] : shaderSources)
		{
			unsigned int shader = glCreateShader(type);
			
			const char* sourceCstr = source.c_str();
			glShaderSource(shader, 1, &sourceCstr, 0);
			glCompileShader(shader);

			int isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled = GL_FALSE)
			{
				int maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

				glDeleteShader(shader);
				std::cerr << "shader compile failed (" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "):\n"
					<< infoLog.data() << std::endl;
				return;
			}
			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}
		glLinkProgram(program);

		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			int maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

			glDeleteProgram(program);
			for (auto id : glShaderIDs) glDeleteShader(id);

			std::cerr << "shader linking failed:\n" << infoLog.data() << std::endl;
			return;
		}
		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
		m_rendererID = program;
	}

	void Shader::bind() const
	{
		glUseProgram(m_rendererID);
	}
	void Shader::unbind() const
	{
		glUseProgram(0);
	}
	int Shader::getUniformLocation(const std::string& name)
	{
		if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
			return m_uniformLocationCache[name];

		int location = glGetUniformLocation(m_rendererID, name.c_str());
		if (location == 1) std::cout << "warning: uniform '" << name << "' not found in shader (" << m_name << ")\n";
	}
}