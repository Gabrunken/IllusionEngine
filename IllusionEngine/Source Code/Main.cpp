/*
* This code will be expanded and organized in this way:
* 
* if there are any classes that don't need to be more than 1, namespace will be used.
* For variables that need to be created inside those namespaces, another anonymous namespace will
* be made, to prevent them to be accessed by the programmer.
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

void CloseWindow(GLFWwindow* window)
{
	glfwHideWindow(window);
}
GLFWwindow* secondaryWindow;

std::vector<GLFWwindow*> windows;

void KeyboardInput(GLFWwindow* window, int key, int scanCode, int action, int mods)
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
		Print("Failed to create window.");
		return nullptr;
	}

	windows.push_back(window);
	return window;
}

int main()
{
	if (!glfwInit())
	{
		Print("GLFW failed to init.");
		return 1;
	}

	GLFWwindow* mainWindow = CreateWindow("Main", 400, 400);

	if (!mainWindow)
	{
		Print("Failed to create main window.");
		return 1;
	}

	secondaryWindow = CreateWindow("Secondary", 200, 200);

	if (!secondaryWindow)
	{
		Print("Failed to create secondary window.");
		return 1;
	}

	glfwSetWindowCloseCallback(secondaryWindow, CloseWindow);
	glfwSetKeyCallback(mainWindow, KeyboardInput);

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();
		glfwSwapBuffers(mainWindow);

		for (int i = 1; i < windows.size(); i++)
		{
			if (!glfwGetWindowAttrib(windows[i], GLFW_VISIBLE)) { continue; }
			glfwSwapBuffers(windows[i]);
		}
	}

	glfwTerminate();

	return 0;
}