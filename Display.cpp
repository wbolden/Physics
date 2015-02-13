#include "Display.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>

#define GLM_FORCE_CUDA
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool glfwInitialized = false;

void safePrint(const GLubyte* input)
{
	if(input == NULL)
	{
		printf("%s\n", "null");
	}
	else
	{
		printf("%s\n", input);
	}
}

void printGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	glErrorCheck();

	std::cout << "Renderer: ";
	safePrint(renderer);
	
	std::cout << "Version: ";
	safePrint(version);
	
	std::cout << "Vendor: ";
	safePrint(vendor);
	
	std::cout << "GLSL Version: ";
	safePrint(glslVersion);
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
		glGetError();
	}


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetWindowPos(window, 1920, 0);

}

GLuint Display::loadTexure(const char* texturePath)
{
	GLuint texture;

	int w, h, comp;
	unsigned char* image = stbi_load(texturePath, &w, &h, &comp, STBI_rgb_alpha);

	if(image == NULL)
	{
		printf("Failed to load image from: %s\n", texturePath);
		return 0;
	}

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

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	printf("Texture loaded: Width: %d, Height: %d, Texture id: %d\n", w, h, texture);

	free(image);

	glErrorCheck();

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

modelInfo Display::createModelVAO(std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<float>& normals)
{
	GLuint vertVBO = 0;
	GLuint texVBO = 0;
	GLuint normVBO = 0;
	GLuint modelVAO = 0;

	glGenBuffers(1, &vertVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glErrorCheck();

	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*texCoords.size(), texCoords.data(), GL_STATIC_DRAW);

	glErrorCheck();

	glGenBuffers(1, &normVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*normals.size(), normals.data(), GL_STATIC_DRAW);

	glErrorCheck();

	std::cout << vertVBO << std::endl;
	std::cout << texVBO << std::endl;
	std::cout << normVBO << std::endl;

	glGenVertexArrays(1, &modelVAO);
	glBindVertexArray(modelVAO);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(1, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normVBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(2, 0);

	glBindVertexArray(0);


	printf("Model loaded: indices: %d, Model VAO: %d\n", (int)(vertices.size()/3), modelVAO);

	glErrorCheck();

	modelInfo info;

	info.vao = modelVAO;
	info.vbos[0] = vertVBO;
	info.vbos[1] = texVBO;
	info.vbos[2] = normVBO;
	info.indices = vertices.size()/3;

	return info;
}

void Display::createInstanceData(GLuint modelVAO, int numBodies, cudaGraphicsResource_t* cudaResources)
{
	GLuint bodyMatVBO;
	GLuint colorVBO;

	glGenBuffers(1, &bodyMatVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bodyMatVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*4*4*numBodies, NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*4*numBodies, NULL, GL_DYNAMIC_DRAW);

	glBindVertexArray(modelVAO);


	for(int i = 3; i < 7; i++)
	{
		glEnableVertexAttribArray(i);
		glBindBuffer(GL_ARRAY_BUFFER, bodyMatVBO);
		glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4*4, (const GLvoid*)((i - 3)*sizeof(float)*4));
		//glVertexAttribPointer(i, 4*4, GL_FLOAT, GL_FALSE, 0, NULL);
		glVertexAttribDivisor(i, 1);
	}


	glEnableVertexAttribArray(7);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(7, 1);

	glBindVertexArray(0);

	glErrorCheck();

	cudaGraphicsGLRegisterBuffer(&cudaResources[0], bodyMatVBO, cudaGraphicsMapFlagsNone);
	cudaGraphicsGLRegisterBuffer(&cudaResources[1], colorVBO, cudaGraphicsMapFlagsNone);
}

modelInfo Display::loadModel(const char* modelPath)
{
	std::ifstream file;
	file.open(modelPath);



	if(!file.is_open())
	{
		printf("Failed to open file from: %s\n", modelPath);
		//return nullptr;
	}

	std::vector<float> vertices;
	std::vector<float> texCoords;
	std::vector<float> normals;

	float x, y, z; 	//vertices
	float u, v;		//texture coordinates
	float i, j, k;	//normals

	while(file >> x)
	{
		file >> y;
		file >> z;

		file >> u;
		file >> v;

		file >> i;
		file >> j;
		file >> k;

		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);

		texCoords.push_back(u);
		texCoords.push_back(v);

		normals.push_back(i);
		normals.push_back(j);
		normals.push_back(k);

		/*
		for(int l = 0; l < 4; l++)
		{

		
		file >> x;
		file >> y;
		file >> z;

		file >> u;
		file >> v;

		file >> i;
		file >> j;
		file >> k;
		}
		*/
	}

	return createModelVAO(vertices, texCoords, normals);
}

Display::Display()
{
	initGL(800, 600, false);
	printGLInfo();
}

Display::Display(int width, int height, bool fullscreen)
{
	initGL(width, height, fullscreen);
	printGLInfo();
}

void Display::startTimer()
{
	startTime = glfwGetTime();
}

void Display::stopTimer()
{
	elapsedTime = glfwGetTime() - startTime;
}

double Display::getTimestep()
{
	return elapsedTime;
}

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

///////////////////////////////////
////////////////////
///////////

float mx = 0.00001;
float my = 0.00001;
float mz = 0;

int count = 0;
glm::quat rotq;

///////////
////////////////////
///////////////////////////////////


void Display::render(RenderInfo& info, Camera* camera)
{	
	//startTimer();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); //wireframe

	if(!info.loaded)
	{
		printf("No data loaded in Scene\n");
		return;
	}

	if(info.rayTrace)
	{

	}
	else
	{
		glUseProgram(info.shaderProgram);
		

		glm::mat4 proj = camera->getProjMatrix();
		glUniformMatrix4fv(info.projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		glm::mat4 view = camera->getViewMatrix();
		glUniformMatrix4fv(info.viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		///////////////////////////////////////////////////
		///////////////////////////////////////////////////====================================
		///////////////////////////////////////////////////
		///////////////////////////////////////////////////==============================
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, info.textures[0]);
/*
			glm::vec3 avel(mx, my, 0.0f);
			

			float angle = glm::length(avel);

			if(angle > 0)
			{
				float sinal = sinf(angle/2.0f)/angle;
				glm::quat inter(cosf(angle/2.0f) ,glm::vec3(avel.x*sinal, avel.y*sinal, avel.z*sinal));
				rotq = inter * rotq;
			}


			glm::mat4 mtest = glm::mat4_cast(rotq);

						mtest = glm::translate(mtest, glm::vec3(1, 5, 8));

printf("MAT\n");
			for(int x= 0; x < 4; x++)
			{

				for(int y = 0; y < 4; y++)
				{
					printf("%f ", mtest[x][y]);
				}
				printf("\n");
			}




			int mloc = glGetUniformLocation(info.shaderProgram, "mtest");
			glUniformMatrix4fv(mloc, 1, GL_FALSE, glm::value_ptr(mtest));
			*/

		///////////////////////////////////////////////////
		///////////////////////////////////////////////////===================
		///////////////////////////////////////////////////=====================================
		///////////////////////////////////////////////////



		glBindVertexArray(info.models[0]);


		//glDrawArrays(GL_LINES, 0, info.indices[0]);

		glDrawArraysInstanced(GL_TRIANGLES, 0, info.indices[0], info.numBodies[0]);

		glfwSwapBuffers(window);
	}

	//stopTimer();
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