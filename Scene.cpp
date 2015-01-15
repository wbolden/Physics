#include "Scene.h"


#include <iostream>
#include <fstream>

Scene::Scene()
{

}



void Scene::loadScene(Display& display, const char* scenePath) //also pass Display&
{
	std::ifstream file;
	file.open(scenePath);

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
	}

	std::cout << display.createModelVAO(vertices, texCoords, normals) << std::endl;
	/*

	for(unsigned int i = 0; i < vertices.size(); i+=3)
	{
*/

}


Scene::~Scene()
{
	
}
