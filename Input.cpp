#include "Input.h"
#include <cstdio>

InputWrapper* InputWrapper::inputInstance;

Input::Input()
{
	setInput();
}

void Input::setWindow(GLFWwindow* window)
{
	glfwSetKeyCallback(window, InputWrapper::keyCallbackDispatch);
	glfwSetCursorPosCallback(window, InputWrapper::cursorPosCallbackDispatch);
	glfwSetMouseButtonCallback(window, InputWrapper::mouseButtonCallbackDispatch);
}

void Input::setWindow(Display& display)
{
	setWindow(display.getWindow());
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//printf("%d\n", key);
	if(key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void Input::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{

}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}

Input::~Input()
{

}