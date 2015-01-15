//#include <iostream>
//#include "Ray.cuh"
//#include "Graphics.h"
//#include "Physics.cuh"
#include "Display.h"
#include "Input.h"
#include "Scene.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glu32.lib") 
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3.lib")

#include <iostream>
int main()
{
	//GLuint texture;

	//glGenTextures(1, &texture);

	Display display = Display();
	Input input = Input();

	input.setWindow(display);

	//GLuint x;
	//GLuint y;
	//GLuint z;

	//glGenBuffers(1, &x);
	//glGenBuffers(1, &y);
	//glGenBuffers(1, &z);

	//std::cout << x << std::endl <<y << std::endl << z << std::endl;

	//Scene scene;
	//scene.loadScene(display, "models/sphere.model");
	//scene.loadScene(display, "models/side.model");
	//std::cout << std::endl;
	//scene.loadScene(display, "models/side.model");
	while(display.running())
	{
		glfwPollEvents();
	}


	return 0;
}
