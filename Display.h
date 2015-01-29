#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>
#include <cstdio>

//class Display;
#include "Camera.h"
#include "Scene.h"

#include <cuda_gl_interop.h>

#define DEBUG

#ifdef DEBUG
	#define glErrorCheck(){ printError(glGetError(), __FILE__, __LINE__); }
#else
	#define glErrorCheck(){}
#endif
inline void printError(GLenum err, const char* file, int line)
{
	if(err != GL_NO_ERROR)
	{
		printf("Error: \"%s\"  File: \"%s\"  Line: \"%d\"\n", gluErrorString(err), file, line);
		//throw 1;
	}
//	throw 1;
}

struct modelInfo
{
	GLuint vao;
	GLuint vbos[3];
	int indices;
};

class Display
{
public:
	Display();
	Display(int width, int height, bool fullscreen);

	GLFWwindow* getWindow();

	bool running();

	GLuint loadTexure(const char* texturePath);

	GLuint initShaders(const char* vshaderPath, const char* fshaderPath);

	modelInfo loadModel(const char* modelPath);

	void createInstanceData(GLuint modelVAO, int numBodies, cudaGraphicsResource_t* cudaResources);

	//void renderScene(Scene& scene) //possibly some other parameters, like user position/camera rotation


	/*
		CUDA texture binding and writing settings
	*/
	void render(RenderInfo& info, Camera* camera);

	void startTimer();
	void stopTimer();

	double getTimestep();

	~Display();

private:

	void initGL(int width, int height, bool fullscreen);
	modelInfo createModelVAO(std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<float>& normals);

	GLFWwindow* window;

	double startTime;
	double elapsedTime;


};




#endif