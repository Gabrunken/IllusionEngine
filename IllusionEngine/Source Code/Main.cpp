/*
* This code will be expanded and organized in this way:
* 
* This program is divided into macro fields that will be organized into sets, for example
* the FileIO inglobates FileSaver and FileLoader, themselves will inglobate other fields.
* 
* It will be expanded like this:
* create a new feature, add it to its corresponding field.
* For example: Texture goes inside Renderer, maybe with some more levels between.
* 
* If a feature has no field, than it doesn't need one. As soon as it needs one
* you create it.
* For example: Window may or may not need an OSCommunication field, you can just leave Window
* as a master field.
*/

#include <glew.h>
#include "Window/Window.hpp"
#include <glm/glm.hpp>

void Print(const char* message)
{
	std::cout << message << std::endl;
}

void SecondaryWindowCloseCallback(GLFWwindow* window)
{
	glfwHideWindow(window);
}
GLFWwindow* secondaryWindow;

std::vector<GLFWwindow*> windows;

void MainWindowKeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_W)
	{
		glfwShowWindow(secondaryWindow);
	}
}

GLFWwindow* CreateWindow(const char* title, int width, int height)
{
	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (!window)
	{
		return nullptr;
	}

	windows.push_back(window);
	return window;
}

void PrintShaderCompilationErrors(unsigned int shaderID, GLenum shaderType)
{
	int compile_result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_result);
	if (compile_result == GL_FALSE)
	{
		int error_length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &error_length);
		char* message = (char*)malloc(error_length * sizeof(char));
		glGetShaderInfoLog(shaderID, error_length, &error_length, message);

		std::cout << "Failed to compile shader of type: ";

		switch (shaderType)
		{
		case GL_VERTEX_SHADER:
			std::cout << "VERTEX.";
			break;

		case GL_FRAGMENT_SHADER:
			std::cout << "FRAGMENT.";
			break;

		default:
			std::cout << "UNKNOWN.";
			break;
		}

		std::cout << std::endl << message << std::endl;

		std::free(message);

		glDeleteShader(shaderID);
	}
}

int main()
{
	if (!glfwInit())
	{
		Print("GLFW failed to init.");
		return 1;
	}

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	GLFWwindow* mainWindow = CreateWindow("Main", 400, 400);

	if (!mainWindow)
	{
		Print("Failed to create main window.");
		return 1;
	}

	glfwMakeContextCurrent(mainWindow);

	if (glewInit() != GLEW_OK)
	{
		Print("Failed to initialize GLEW.");
		return 1;
	}

	secondaryWindow = CreateWindow("Secondary", 200, 200);

	if (!secondaryWindow)
	{
		Print("Failed to create secondary window.");
		return 1;
	}

	glfwSetWindowCloseCallback(secondaryWindow, SecondaryWindowCloseCallback);
	glfwSetKeyCallback(mainWindow, MainWindowKeyboardCallback);

	float vertices[] = {
		-0.5, -0.5,
		0.5, -0.5,
		0.5, 0.5,
		-0.5, 0.5
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);
	
	const char* vertexSource = "#version 440 core\n"

		"in vec4 inVertexPosition;\n"

		"void main()\n"
		"{\n"
		"	gl_Position = inVertexPosition;\n"
		"}\n";

	const char* fragmentSource = "#version 440 core\n"

		"out vec4 outColor;\n"

		"uniform vec3 inColor;\n"

		"void main()\n"
		"{\n"
		"	outColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
		"}\n";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);
	PrintShaderCompilationErrors(vertexShader, GL_VERTEX_SHADER);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);
	PrintShaderCompilationErrors(fragmentShader, GL_FRAGMENT_SHADER);

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	glUseProgram(shaderProgram);

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

		glfwMakeContextCurrent(mainWindow);
		glClearColor(1.0f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//6 is the amount of indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glfwSwapBuffers(mainWindow);

		for (int i = 1; i < windows.size(); i++)
		{
			//check if the window is not visible (hidden), because if it is, then don't render nor do anything else
			if (!glfwGetWindowAttrib(windows[i], GLFW_VISIBLE)) { continue; }
			glfwMakeContextCurrent(windows[i]);
			glClearColor(0.2f, 0.5f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwSwapBuffers(windows[i]);
		}
	}

	glfwTerminate();

	return 0;
}