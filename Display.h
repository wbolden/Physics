#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>

class Display
{
public:
	Display();
	Display(int width, int height, bool fullscreen);

	GLFWwindow* getWindow();

	bool running();

	GLuint loadTexure(const char* texturePath);

	GLuint initShaders(const char* vshaderPath, const char* fshaderPath);

	GLuint createModelVAO(std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<float>& normals);


	//void renderScene(Scene& scene) //possibly some other parameters, like user position/camera rotation


	/*
		CUDA texture binding and writing settings


	*/


	~Display();

private:

	void initGL(int width, int height, bool fullscreen);

	GLFWwindow* window;
};




#endif