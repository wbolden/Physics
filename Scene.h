#ifndef SCENE_H
#define SCENE_H
//#include "Display.h"
#include <vector>
#include "Body.h"

#include "GLFW/glfw3.h"



// #include "cuda.h"
// #include "cuda_runtime.h"
// #include "device_launch_parameters.h"

class Display;

class Scene
{
public:

	Scene();
	Scene(Display& display, const char* scenePath, bool physics, bool collision, bool rayTracing);
//	Scene(Display& display, const char* scenePath) : Scene(display, scenePath, true, true, false);

	void loadScene(Display& display, const char* scenePath);

	bool usePhysics();
	bool useCollision();
	bool useRayTracing();
	bool hasData();



	~Scene();

private:
	bool loaded;
	bool physics;
	bool collision;
	bool rayTracing;

//std::vector<cudaGraphicsResource_t> cudaResources;
std::vector<GLuint> modelVAOs;
};


#endif