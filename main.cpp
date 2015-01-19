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
	Display display = Display(800, 600, false);
	
	Input input = Input();
	input.setWindow(display);

	printf("\n");
	Scene scene;
	scene.loadScene(display, "scenes/s1.scene");
	printf("\n");
	//scene.loadScene(display, "models/side.model");
	//std::cout << std::endl;
	//scene.loadScene(display, "models/side.model");
	while(display.running() && false)
	{
		glfwPollEvents();
	}


	return 0;
}
