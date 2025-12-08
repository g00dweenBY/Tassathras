#include "Core/Application.h"
#include <iostream>

int main()
{
	try
	{
		Application app;
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "unhandled exception: " << e.what() << std::endl;
		return -1;
	}
	catch(...)
	{
		std::cerr << "unkonwn exception.\n";
		return -1;
	}

	return 0;
}

/*
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace Tassathras
{
}

std::string readShaderFile(const char* filePath)
{
	std::ifstream shaderFile(filePath, std::ios::in);

	if (!shaderFile.is_open())
	{
		std::cerr << "Error: cant open file " << filePath << std::endl;
		return "";
	}

	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();

	return shaderStream.str();
}

GLuint compileShader(GLenum type, const char* source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint success;
	char infolog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, infolog);
		std::cerr << "error compile shader:\n" << infolog << std::endl;

		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource)
{
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	if (vertexShader == 0) return 0;

	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
	if (fragmentShader == 0)
	{
		glDeleteShader(vertexShader);
		return 0;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	GLint success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cerr << "error link program:\n" << infoLog << std::endl;

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(program);
		return 0;
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void glfwErrorCallback(int error, const char* description)
{
	std::cerr << "glfw error: " << error << ": " << description << std::endl;
}

void framebufferSizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

int main()
{
	std::cout << "init glfw\n";
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwInit())
	{
		std::cerr << "error in main glfw'\n";
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "triangle", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "error window glfw\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	std::cout << "init glad\n";
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "can't init glad\n";
		glfwTerminate();
		return -1;
	}

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	std::cout << "opengl: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "card: " << glGetString(GL_RENDERER) << std::endl;

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,     0.0f, 0.0f, 1.0f,

		0.2f, -0.5f, 0.0f,    1.0f, 1.0f, 0.0f,
		0.6f, -0.5f, 0.0f,    0.0f, 1.0f, 1.0f,
		0.4f, 0.2f, 0.0f,     1.0f, 0.0f, 1.0f,

		0.4f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,
		0.8f, 0.5f, 0.0f,    0.0f, 1.0f, 1.0f,
		0.2f, 0.2f, 0.0f,     1.0f, 0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(float),
		(void*)0
	);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(float),
		(void*)(3 * sizeof(float))
	);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	std::string vertexShaderSource = readShaderFile("assets/shaders/vertex.glsl");
	std::string fragmentShaderSource = readShaderFile("assets/shaders/fragment.glsl");

	if (vertexShaderSource.empty() || fragmentShaderSource.empty())
	{
		std::cerr << "can't upload shaders\n";
		glfwTerminate();
		return -1;
	}
	GLuint shaderProgram = createShaderProgram(
		vertexShaderSource.c_str(),
		fragmentShaderSource.c_str()
	);

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glfwDestroyWindow(window);
	glfwTerminate();

	std::cout << "END\n";


}
*/