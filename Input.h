#ifndef INPUT_H
#define INPUT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Display.h"

class InputWrapper
{
public:
	virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
	virtual void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) = 0;
	virtual void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) = 0;

	static InputWrapper* inputInstance;

	virtual void setInput()
	{
		inputInstance = this;
	}

	static void keyCallbackDispatch(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if(inputInstance)
		{
			inputInstance->keyCallback(window, key, scancode, action, mods);
		}
	}

	static void cursorPosCallbackDispatch(GLFWwindow* window, double xpos, double ypos)
	{
		if(inputInstance)
		{
			inputInstance->cursorPosCallback(window, xpos, ypos);
		}
	}

	static void mouseButtonCallbackDispatch(GLFWwindow* window, int button, int action, int mods)
	{
		if(inputInstance)
		{
			inputInstance->mouseButtonCallback(window, button, action, mods);
		}
	}
	
};

class Input : InputWrapper
{
public:
	Input();

	void setWindow(GLFWwindow* window);

	void setWindow(Display& display);

	~Input();

	virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	virtual void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	virtual void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};

#endif