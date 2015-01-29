#ifndef SCENE_H
#define SCENE_H
//#include "Display.h"
#include <vector>
#include "Body.h"

#include "GLFW/glfw3.h"
#include <cuda_gl_interop.h>


// #include "cuda.h"
// #include "cuda_runtime.h"
// #include "device_launch_parameters.h"

struct RenderInfo
{
	bool rayTrace;
	bool loaded;
	bool collision;
	bool physics;

	GLuint shaderProgram;
	GLuint projLoc;
	GLuint viewLoc;
	std::vector<GLuint> models;
	std::vector<GLuint> textures;
	std::vector<int> indices;
	std::vector<int> numBodies;
	cudaGraphicsResource_t cudaResources[2];
};

struct glData
{
	glm::mat4* modelRT;
	glm::vec4* color;
};

class Display;

class Scene
{
public:

	Scene();
	Scene(Display& display, const char* scenePath, const char* vShader, const char* fShader, bool physics, bool collision, bool rayTracing);
//	Scene(Display& display, const char* scenePath) : Scene(display, scenePath, true, true, false);

	void loadScene(Display& display, const char* scenePath);

	//
//check for errors
	//GLuint getShaderProgram();
	//GLuint getModelVAO(); //to be replaced

	Body* getCUDABodies();
	glData getCUDAResourcePointers();
	void unmapCUDAResources();
	void unregisterCUDA();
	
	RenderInfo getRenderInfo();




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

	Body* bodies;
	Body* devBodies;



	//GLuint shaderProgram;
	//GLuint mvao;

//std::vector<cudaGraphicsResource_t> cudaResources;
//std::vector<GLuint> modelVAOs;

	RenderInfo info;
};


#endif