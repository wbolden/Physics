#include "Input.h"
#include <cstdio>

InputWrapper* InputWrapper::inputInstance;

Input::Input()
{
	setInput();
	camera = nullptr;
}

Input::Input(Camera* camera)
{
	setInput();
	setCamera(camera);
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

bool Input::paused()
{
	return pause;
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//printf("%d\n", key);
	if(key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if(camera != nullptr)
	{

		if(mods & GLFW_MOD_SHIFT)
		{
			camera->setBoost(10);
		}
		else
		{
			camera->setBoost(1);
		}

		if(key == (GLFW_KEY_SPACE))
		{

			if(action == GLFW_PRESS)
			{
				pause = true;
			}
			else if (action == GLFW_RELEASE)
			{
				pause = false;
			}
		}


		if(key == (GLFW_KEY_W))
		{

			if(action == GLFW_PRESS)
			{
				camera->setWS(1);
				lastWS = key;
			}
			else if (action == GLFW_RELEASE && lastWS == key)
			{
				camera->setWS(0);
			}
		}
		if(key == (GLFW_KEY_S))
		{
			if(action == GLFW_PRESS)
			{
				camera->setWS(-1);
				lastWS = key;
			}
			else if (action == GLFW_RELEASE && lastWS == key)
			{
				camera->setWS(0);
			}

			
		}

		if(key == (GLFW_KEY_A))
		{
			if(action == GLFW_PRESS)
			{
				camera->setAD(1);
				lastAD = key;
			}
			else if (action == GLFW_RELEASE && lastAD == key)
			{
				camera->setAD(0);
			}
		}
		if(key == (GLFW_KEY_D))
		{
			if(action == GLFW_PRESS)
			{
				camera->setAD(-1);
				lastAD = key;
			}
			else if (action == GLFW_RELEASE && lastAD == key)
			{
				camera->setAD(0);
			}
		}

		if(key == (GLFW_KEY_UP))
		{
			if(action == GLFW_PRESS)
			{
				camera->setUD(1);
				lastUD = key;
			}
			else if (action == GLFW_RELEASE && lastUD == key)
			{
				camera->setUD(0);
			}
		}
		if(key == (GLFW_KEY_DOWN))
		{
			if(action == GLFW_PRESS)
			{
				camera->setUD(-1);
				lastUD = key;
			}
			else if (action == GLFW_RELEASE && lastUD == key)
			{
				camera->setUD(0);
			}
		}
		if(key == (GLFW_KEY_RIGHT))
		{
			if(action == GLFW_PRESS)
			{
				camera->setRL(1);
				lastRL = key;
			}
			else if (action == GLFW_RELEASE && lastRL == key)
			{
				camera->setRL(0);
			}
		}
		if(key == (GLFW_KEY_LEFT))
		{
			if(action == GLFW_PRESS)
			{
				camera->setRL(-1);
				lastRL = key;
			}
			else if (action == GLFW_RELEASE && lastRL == key)
			{
				camera->setRL(0);
			}
		}
	}
}

void Input::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{

}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}

void Input::setCamera(Camera* camera)
{
	this->camera = camera;
}

Input::~Input()
{

}