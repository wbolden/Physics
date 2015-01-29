//#include <iostream>
//#include "Ray.cuh"
//#include "Graphics.h"
//#include "Physics.cuh"
#include "Display.h"
#include "Input.h"
#include "Scene.h"
#include "Camera.h"
#include "Physics.cuh"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glu32.lib") 
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3.lib")


#include <iostream>
int main()
{
	Display display(800, 600, false);
	
	Camera* camera = new Camera();

	Input input(camera);
	input.setWindow(display);



	Scene scene(display, "scenes/s1.scene", "shaders/vert.glsl", "shaders/frag.glsl", true, true, false);
/*

		RenderInfo info = scene.getRenderInfo();
		glData data = scene.getCUDAResourcePointers();

		runPhysics(scene.getCUDABodies(), data.modelRT, data.color, info.numBodies[0], 0);
		
		scene.unmapCUDAResources();
		scene.unregisterCUDA();
*/

	while(display.running() && true)
	{
		float timestep = display.getTimestep();

		display.startTimer();

		RenderInfo info = scene.getRenderInfo();
		glData data = scene.getCUDAResourcePointers();

		runPhysics(scene.getCUDABodies(), data.modelRT, data.color, info.numBodies[0], timestep);

		scene.unmapCUDAResources();

		//see if cuda can make rotations correctly then add physics

		display.render(info, camera);
		glfwPollEvents();
		camera->update(timestep);
		//printf("%f\n", display.getRenderTime());

		display.stopTimer();
	}

	delete camera;
	return 0;
}
