#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>
#include <cstdio>

//class Display;

#include "Scene.h"

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
	}
}

class Display
{
public:
	Display();
	Display(int width, int height, bool fullscreen);

	GLFWwindow* getWindow();

	bool running();

	GLuint loadTexure(const char* texturePath);

	GLuint initShaders(const char* vshaderPath, const char* fshaderPath);

	GLuint loadModel(const char* modelPath);

	//void renderScene(Scene& scene) //possibly some other parameters, like user position/camera rotation


	/*
		CUDA texture binding and writing settings
	*/
	void render(Scene& scene);

	~Display();

private:

	void initGL(int width, int height, bool fullscreen);
	GLuint createModelVAO(std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<float>& normals);



	GLFWwindow* window;
};




#endif