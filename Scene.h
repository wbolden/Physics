#ifndef SCENE_H
#define SCENE_H
#include "Display.h"
#include <vector>

#include "GLFW/glfw3.h"

#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

class Scene
{
public:

	Scene();

	void loadScene(Display& display ,const char* scenePath);

	~Scene();

private:

std::vector<cudaGraphicsResource_t> cudaResources;
std::vector<GLuint> modelVAOs;
};


#endif