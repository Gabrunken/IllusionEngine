#include <glfw3.h>
#include <iostream>
#include <vector>

class Window
{
public:
	Window() = delete;
	~Window() = delete;

	//Create a new Window, returns the ID of that window.
	//If the ID is negative, it is an invalid window ID.
	static int8_t Create(const char* title, uint16_t width, uint16_t height);
	static void Update(int8_t windowID);
	static bool IsValid(int8_t windowID);
private:
	//IDs made by the Create() method are sequential, starting from 0.
	//When CloseCallback() is called, the window at index windowID is
	//set to nullptr.
	static std::vector<GLFWwindow*> glfwWindows;

	static void CloseCallback(int8_t windowID);
};
