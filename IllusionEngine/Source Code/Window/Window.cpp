#include "Window.hpp"

std::vector<GLFWwindow*> Window::glfwWindows;

int8_t Window::Create(const char* title, uint16_t width, uint16_t height)
{
	if (!glfwInit())
	{
		//Failed to initialize GLFW
		std::cout << "Failed to initialize GLFW." << std::endl;
		std::cout << "Failed to create a window, due to GLFW not being initialized." << std::endl;
		return -1;
	}

	//Add the newly created GLFWwindow* to the static list of windows
	glfwWindows.push_back(glfwCreateWindow(width, height, title, nullptr, nullptr));
	
	if (!glfwWindows.back())
	{
		//Window created incorrectly
		glfwWindows.pop_back();
		std::cout << "Failed to create window of title " << title << "." << std::endl;
		return -1;
	}

	//Set callbacks
	glfwSetWindowCloseCallback(glfwWindows.back(), [](GLFWwindow* window) {CloseCallback(glfwWindows.size() - 1); });

	//Return the id of the created window
	return glfwWindows.size() - 1;
}

void Window::Update(int8_t windowID)
{
	if (glfwWindows[windowID] == nullptr)
	{
		return;
	}

	if (!glfwWindowShouldClose(glfwWindows[windowID]))
	{
		glfwPollEvents();

		if (glfwWindows[windowID] == nullptr)
		{
			//A quick check if the window is not invalid, it might be because
			//glfwPollEvents() might have called CloseCallkack() which removes windowID
			//from glfwWindows, so here we don't use nullptr as an argument.
			return;
		}

		glfwSwapBuffers(glfwWindows[windowID]);
	}
}

bool Window::IsValid(int8_t windowID)
{
	return glfwWindows[windowID] != nullptr;
}

void Window::CloseCallback(int8_t windowID)
{
	glfwWindows[windowID] = nullptr;
}
