#include "Display.h"



#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>


bool glfwInitialized = false;

void printfGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDER);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	
	std::cout << "Renderer: ";
	if(renderer == NULL)
	{
		std::cout << "null" << std::endl;
	}
	else
	{
		std::cout << renderer << std::endl;
	}
	
	std::cout << "Version: ";
	if(version == NULL)
	{
		std::cout << "null" << std::endl;
	}
	else
	{
		std::cout << version << std::endl;
	}
	
	std::cout << "Vendor: ";
	if(vendor == NULL)
	{
		std::cout << "null" << std::endl;
	}
	else
	{
		std::cout << vendor << std::endl;
	}
	
	std::cout << "GLSL Version: ";
	if(glslVersion == NULL)
	{
		std::cout << "null" << std::endl;
	}
	else
	{
		std::cout << glslVersion << std::endl;
	}
}

void Display::initGL(int width, int height, bool fullscreen)
{
	glfwInitialized = glfwInit();

	if(!glfwInitialized)
	{
		std::cout << "GLFW initialization failed" << std::endl;
		return;
	}
	glfwWindowHint(GLFW_RESIZABLE, false);

	std::cout << "GLFW initialized" << std::endl;

	glfwWindowHint(GLFW_SAMPLES, 16);

	if(fullscreen)
	{
		window = glfwCreateWindow(width, height, "Physics", glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		window = glfwCreateWindow(width, height, "Physics", NULL, NULL);
	}

	if(!window)
	{
		std::cout << "Window creation failed" << std::endl;
		return;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cout << "GLEW initialization failed" << std::endl;
	}
	else
	{
		std::cout << "GLEW initialized" << std::endl;
	}
}

GLuint Display::loadTexure(const char* texturePath)
{
	GLuint texture;

	int w, h, comp;
	unsigned char* image = stbi_load(texturePath, &w, &h, &comp, STBI_rgb_alpha);

	//Flip
	int wbyte = w*4;
	unsigned char *top = NULL;
	unsigned char* bottom = NULL;
	unsigned char temp = 0;
	int hh = h/2;

	for(int r = 0; r < hh; r++)
	{
		top = image + r * wbyte;
		bottom = image + (h - r - 1) * wbyte;
		for(int col = 0; col < wbyte; col++)
		{
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}
	//flip
	
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	free(image);

	return texture;
}

const char* loadShader(const char* filename)
{
	std::ifstream file;
	file.open(filename);

	std::string shader = "";

	while(!file.eof())
	{
		std::string temp;
		std::getline(file, temp);
		shader += temp;
		shader += '\n';
	}

	char* data = new char[shader.length()+1];
	memcpy(data, shader.c_str(), shader.length()*sizeof(char));
	data[shader.length()] = '\0';

	return data;
}

bool shaderCompiled(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	return (bool) status;
}

bool compileShader(GLuint shader, const GLchar* shaderSource)
{
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	return shaderCompiled(shader);
}

GLuint Display::initShaders(const char* vshaderPath, const char* fshaderPath)
{
	const char* vshader = loadShader(vshaderPath);
	const char* fshader = loadShader(fshaderPath);


	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);



	if(!compileShader(vs, (const GLchar*)vshader))
	{
		std::cout << "Failed to load Vertex Shader from " << vshaderPath << std::endl;

		GLchar* info = new GLchar[10000];
		glGetShaderInfoLog(vs, 10000, NULL, info);
		printf("%s\n", info);
		delete[] info;
	}
	else
	{
		std::cout << "Vertex Shader loaded from " << vshaderPath << std::endl;
	}

	if(!compileShader(fs, (const GLchar*)fshader))
	{
		std::cout << "Failed to load Fragment Shader " << fshaderPath << std::endl;

		GLchar* info = new GLchar[10000];
		glGetShaderInfoLog(fs, 10000, NULL, info);
		printf("%s\n", info);
		delete[] info;
	}
	else
	{
		std::cout << "Fragment Shader loaded from " << fshaderPath << std::endl;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fs);
	glAttachShader(shaderProgram, vs);
	glLinkProgram(shaderProgram);

	return shaderProgram;
}

GLuint Display::createModelVAO(std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<float>& normals)
{
	GLuint vertVBO = 1110;
	GLuint texVBO = -1110;
	GLuint normVBO = 1110;

	GLuint sceneVAO = 1110;

	std::cout << vertices.size() << std::endl;
	std::cout << texCoords.size() << std::endl;
	std::cout << normals.size() << std::endl;

	std::cout << vertices.data() << std::endl;
	float* dat = vertices.data();

	std::cout << dat[0] << "," << dat[1] << "," << dat[2]<<  std::endl;


	if(glIsBuffer(vertVBO) == GL_TRUE)
	{
	 	std::cout << "true"<< std::endl;
	}
	else
	{
	 	std::cout << "false"<< std::endl;
	}
	std::cout << gluErrorString( glGetError()) << std::endl;

	glGenBuffers(1, &vertVBO);
std::cout << gluErrorString( glGetError()) << std::endl;

	if(glIsBuffer(vertVBO) == GL_TRUE)
	{
	 	std::cout << "true"<< std::endl;
	}
	else
	{
	 	std::cout << "false"<< std::endl;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertVBO);

	
std::cout << gluErrorString( glGetError()) << std::endl;

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

	if(glIsBuffer(vertVBO) == GL_TRUE)
	{
		std::cout << "true"<< std::endl;
	}
	else
	{
	 	std::cout << "false"<< std::endl;
	}

	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*texCoords.size(), texCoords.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &normVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*normals.size(), normals.data(), GL_STATIC_DRAW);

	std::cout << gluErrorString( glGetError()) << std::endl;

	std::cout << vertVBO << std::endl;
	std::cout << texVBO << std::endl;
	std::cout << normVBO << std::endl;

	glGenVertexArrays(1, &sceneVAO);
	glBindVertexArray(sceneVAO);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normVBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	std::cout << gluErrorString( glGetError()) << std::endl;

	return sceneVAO;
}


Display::Display()
{
	Display(800, 600, false);
}

Display::Display(int width, int height, bool fullscreen)
{
	initGL(width, height, fullscreen);
	printfGLInfo();
}

bool Display::running()
{
	return !glfwWindowShouldClose(window);
}

GLFWwindow* Display::getWindow()
{
	return window;
}

Display::~Display()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}