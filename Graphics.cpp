#include "Graphics.h"

#include <iostream>
#include <string>
#include <fstream>
//#include <cctype>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


GLuint loadTexure(const char* texturePath)
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
//	glActiveTexture(GL_TEXTURE0);
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

	int length = shader.length();

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

GLuint initShaders(const char* vshaderPath, const char* fshaderPath)
{
	//const char* vshader = loadShader("vert.glsl");
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
