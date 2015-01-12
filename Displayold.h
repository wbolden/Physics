#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#define GLM_FORCE_RADIANS
//#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




class Display
{
public:
	Display(void);

	GLuint initShaders(const char* vshaderPath, const char* fshaderPath);
	GLuint loadTexure(const char* texturePath);


//	void setVertexData(); //TODO
//	bool running();
//	bool paused();
	//void render(GLFWwindow* window);
//	void render(GLFWwindow* window, GLuint tvao, int numVerts);
	void render(GLuint vao, int numVerts, int shaderProgram);
//	void renderText(Text& text, float* sColor, float* rColor, int active, GLuint guiShader, GLuint textShader, GLuint texture);
//	void renderScene(Scene& scene, GLuint sceneShader);
//	void renderFadeScene(Scene& scene1, Scene& scene2, GLuint fadeShader, float fade, float focus);

//	void Display::renderButton(GLuint bvao, int numVerts, GLuint buttonShader, float* color);

	~Display(void);
};

#endif

