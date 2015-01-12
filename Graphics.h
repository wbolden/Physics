#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>


struct renderInfo{
	GLuint vao;
	int numVerts;
	GLuint shaderProgram;
};

struct model{
	int numElements;
	GLuint vertices;
	GLuint texCoords;
	GLuint normals;
	GLuint texture; // have vector of textures somewhere?SSS
};

GLuint initShaders(const char* vShaderPath, const char* fShaderPath);
GLuint loadTexture(const char* texturePath);
model loadModel(const char* modelPath);



void render(renderInfo info);

#endif