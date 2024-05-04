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

int main()
{
	int8_t mainWindow = Window::Create("Illusion", 1280, 720);
	int8_t secondWindow = Window::Create("Second", 400, 400);
	//int8_t thirdWindow = Window::Create("Third", 640, 480);

	while (Window::IsValid(mainWindow) || Window::IsValid(secondWindow))
	{
		Window::Update(mainWindow);
		Window::Update(secondWindow);

	}

	return 0;
}