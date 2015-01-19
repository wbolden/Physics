#include "Display.h"
#include "Scene.h"


#include <iostream> 
#include <sstream> //stringstream
#include <fstream> 

#include <stdlib.h> //for rand
#include <time.h> //for time(0)

Scene::Scene()
{
	loaded = false;
	physics = true;
	collision = true;
	rayTracing = false;
}


Scene::Scene(Display& display, const char* scenePath, bool physics, bool collision, bool rayTracing)
{

}

bool Scene::hasData()
{
	return loaded;
}

bool Scene::usePhysics()
{
	return physics;
}

bool Scene::useCollision()
{
	return collision;
}

bool Scene::useRayTracing()
{
	return rayTracing;
}

struct body
{

};

struct range
{
	unsigned int start;
	unsigned int end;

	range(unsigned int s, unsigned int e)
	{
		start = s;
		end = e;
	}
};

std::vector<range> getRanges(std::vector<std::string>& lines, const char* start, const char* end)
{
	std::vector<range> ranges;
	for(unsigned int i = 0; i < lines.size(); i++)
	{
		if(lines[i].find(start) != std::string::npos) //Get lines from #texture
		{
			range vectorRange(i, i);
			while(lines[i].find(end) == std::string::npos) //to #end
			{
				vectorRange.end = i;
				i++;
			}
			ranges.push_back(vectorRange);
		}
	}
	return ranges;
}

void printRangeVector(std::vector<std::string>& lines, std::vector<range>& ranges)
{
	for(unsigned int w = 0; w < ranges.size(); w++)
	{
		for(unsigned int i = ranges[w].start; i <= ranges[w].end; i++)
		{
			printf("%s\n", lines[i].data());
		}
		printf("\n");
	}
}

int getIdentiferIndex(std::vector<std::string>& lines, std::vector<range>& identifierRange, unsigned int line)
{
	std::string identifier;
	std::stringstream ss(lines[line]);
	//ss.ignore(1024, ' ');
	ss >> identifier;

	//printf("%s\n", identifier.data());

	for(unsigned int i = 0;  i < identifierRange.size(); i++)
	{
		if(lines[identifierRange[i].start].find(identifier.data()) != std::string::npos) //If identifier was found
		{
			 return (int)i;
		}
	}
	return -1;
}


void Scene::loadScene(Display& display, const char* scenePath) //also pass Display&
{
	std::vector<std::string> lines;

	std::ifstream file;
	file.open(scenePath);

	if(!file.is_open())
	{
		printf("Failed to open file %s\n", scenePath);
		return;
	}

	std::string line;
	while(std::getline(file, line))
	{
		line = line.substr(0, line.find("//")); //Remove comments
		if(line.find_first_not_of("\t\n ") != std::string::npos) //Don't add blank lines
		{
			lines.push_back(line);
		}
	}

	std::vector<range> texRanges = getRanges(lines, "#texture", "#end");
	std::vector<range> modelRanges = getRanges(lines, "#model", "#end");
	std::vector<range> bodyRanges = getRanges(lines, "#body", "#end");
//	std::vector<range> sceneRanges = getRanges(lines, "#scene", "#end");
//printf("%d\n", bodyRanges[0].end);
	printRangeVector(lines, texRanges);
	printRangeVector(lines, modelRanges);
	printRangeVector(lines, bodyRanges);
//	printRangeVector(lines, sceneRanges);

	//Load textures
	std::vector<GLuint> textures;
	for(unsigned int i = 0; i < texRanges.size(); i++)
	{
		GLuint tex;
		int stringIndex = texRanges[i].start+1;  //(+1) - First line is #texture identifier, second line is file path
		tex = display.loadTexure(lines[stringIndex].data());
		if(tex != 0)
		{
			textures.push_back(tex);
		}
	}

	//Load models
	std::vector<GLuint> models;
	for(unsigned int i = 0; i < modelRanges.size(); i++)
	{
		GLuint vao;
		int stringIndex = modelRanges[i].start+1;  //(+1) - First line is #model identifier, second line is file path
		vao = display.loadModel(lines[stringIndex].data());
		if(vao != 0)
		{
			models.push_back(vao);
		}
	}

	std::vector<Body> bodies;

	for(unsigned int i = 0; i < bodyRanges.size(); i++)
	{
		int modelIndex = getIdentiferIndex(lines, modelRanges, bodyRanges[i].start+1);
		int textureIndex = getIdentiferIndex(lines, modelRanges, bodyRanges[i].start+2);
		int modelId = models[modelIndex];
		int textureId = textures[textureIndex];

		std::stringstream ss;
		for(unsigned int r = bodyRanges[i].start + 3; r <= bodyRanges[i].end; r++)
		{
			ss << lines[r];
			ss << ' ';
		}

		std::cout << ss.str() << std::endl;
	//	printf("%d\n", bodyRanges[i].end);

		float mass;
		float scale;
		float3 com;
		float3 rot;
		float3 vel;
		float3 avel;

		float3 iten[3];

		ss >> mass;
		ss >> scale;

		ss >> com.x;
		ss >> com.y;
		ss >> com.z;

		ss >> rot.x;
		ss >> rot.y;
		ss >> rot.z;

		ss >> vel.x;
		ss >> vel.y;
		ss >> vel.z;

		ss >> avel.x;
		ss >> avel.y;
		ss >> avel.z;		

		for(int r = 0; r < 3; r++)
		{
			ss >> iten[r].x;
			ss >> iten[r].y;
			ss >> iten[r].z;	
		}

		printf("%f\n", mass);
		printf("%f\n", scale);
		printf("com: %f, %f, %f\n", com.x, com.y, com.z);
		printf("rot:%f, %f, %f\n", rot.x, rot.y, rot.z);
		printf("vel: %f, %f, %f\n", vel.x, vel.y, vel.z);
		printf("avel: %f, %f, %f\n\n", avel.x, avel.y, avel.z);

		printf("%f, %f, %f\n", iten[0].x, iten[0].y, iten[0].z);
		printf("%f, %f, %f\n", iten[1].x, iten[1].y, iten[1].z);
		printf("%f, %f, %f\n", iten[2].x, iten[2].y, iten[2].z);
		//Mat itensor

		int numBodies;
		int min[3];
		int max[3];
		int range[3];


		ss >> numBodies;
		for(int r = 0; r < 3; r++)
		{
			ss >> min[r];
			ss >> max[r];
			range[r] = max[r] - min[r];
		}

		printf("%d\n", numBodies);

		printf("%d, %d\n", min[0], max[0]);
		printf("%d, %d\n", min[1], max[1]);		
		printf("%d, %d\n", min[2], max[2]);

		srand(time(0));
		for(int n = 0; n < numBodies; n++)
		{
			float3 pos;

			pos.x = (float)((rand() % range[0]) + min[0]); 
			pos.y = (float)((rand() % range[1]) + min[1]);
			pos.z = (float)((rand() % range[2]) + min[2]);


			bodies.push_back(Body(modelId, textureId, mass, pos, rot, vel, avel));
			printf("Body %d added\n", n);

		}

	}


	
}


Scene::~Scene()
{
	
}
