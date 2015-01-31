#include "Display.h"
#include "Scene.h"


#include <iostream> 
#include <sstream> //stringstream
#include <fstream> 

#include <stdlib.h> //for rand
#include <time.h> //for time(0)

#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

Scene::Scene()
{
	info.loaded = false;
	info.physics = true;
	info.collision = true;
	info.rayTrace = false;
	info.shaderProgram = 0;
	//info.projLoc =  glGetUniformLocation(shaderProgram, "proj");
	//info.viewLoc =  glGetUniformLocation(shaderProgram, "view");
}


Scene::Scene(Display& display, const char* scenePath, const char* vShader, const char* fShader, bool physics, bool collision, bool rayTracing)
{
	info.loaded = false;
	info.physics = physics;
	info.collision = collision;
	info.rayTrace = rayTracing;
	info.shaderProgram = display.initShaders(vShader, fShader);
	info.projLoc =  glGetUniformLocation(info.shaderProgram, "proj");
	info.viewLoc =  glGetUniformLocation(info.shaderProgram, "view");

	printf("%d\n", info.projLoc);
	printf("%d\n", info.viewLoc);

	loadScene(display, scenePath);
}


bool Scene::hasData()
{
	return info.loaded;
}

bool Scene::usePhysics()
{
	return info.physics;
}

bool Scene::useCollision()
{
	return info.collision;
}

bool Scene::useRayTracing()
{
	return info.rayTrace;
}

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

void printMinMax(float* min, float* max)
{
	printf("x: [%f, %f]\n", min[0], max[0]);
	printf("y: [%f, %f]\n", min[1], max[1]);		
	printf("z: [%f, %f]\n", min[2], max[2]);
}


float randRange(float min, float max)
{
	float range = max - min;

	float scale = (float)RAND_MAX/range;
	return rand()/scale + min;
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


	//Load textures
	//std::vector<GLuint> textures;
	for(unsigned int i = 0; i < texRanges.size(); i++)
	{
		GLuint tex;
		int stringIndex = texRanges[i].start+1;  //(+1) - First line is #texture identifier, second line is file path
		tex = display.loadTexure(lines[stringIndex].data());
		if(tex != 0)
		{
			info.textures.push_back(tex);
		}
	}

	//Load models

	for(unsigned int i = 0; i < modelRanges.size(); i++)
	{
		modelInfo minfo;
		int stringIndex = modelRanges[i].start+1;  //(+1) - First line is #model identifier, second line is file path
		minfo = display.loadModel(lines[stringIndex].data());
		if(minfo.vao != 0)
		{
			info.models.push_back(minfo.vao);

			info.indices.push_back(minfo.indices);
		}
	}


	std::vector<Body> bodies;

	for(unsigned int i = 0; i < bodyRanges.size(); i++)
	{
		int modelIndex = getIdentiferIndex(lines, modelRanges, bodyRanges[i].start+1);
		int textureIndex = getIdentiferIndex(lines, modelRanges, bodyRanges[i].start+2);
		//int modelId = info.models[modelIndex];
		//int textureId = info.textures[textureIndex];

		std::stringstream ss;
		for(unsigned int r = bodyRanges[i].start + 3; r <= bodyRanges[i].end; r++)
		{
			ss << lines[r];
			ss << ' ';
		}

		float mass;
		float scale;
		float3 com;


		float3 iten[3];

		ss >> mass;
		ss >> scale;

		ss >> com.x;
		ss >> com.y;
		ss >> com.z;

		for(int r = 0; r < 3; r++)
		{
			ss >> iten[r].x;
			ss >> iten[r].y;
			ss >> iten[r].z;	
		}

		printf("\nBody %d loaded:\n\n", i);

		printf("Mass: %f\n", mass);
		printf("Scale: %f\n", scale);
		printf("Center of Mass: %f, %f, %f\n", com.x, com.y, com.z);

		printf("\nInetia tensor constants:\n");
		printf("%f, %f, %f\n", iten[0].x, iten[0].y, iten[0].z);
		printf("%f, %f, %f\n", iten[1].x, iten[1].y, iten[1].z);
		printf("%f, %f, %f\n", iten[2].x, iten[2].y, iten[2].z);

		//m*scale^2 works for sphere
		glm::mat3 inertiaTensor(glm::vec3(iten[0].x, iten[0].y, iten[0].z)*mass*scale*scale,
								glm::vec3(iten[1].x, iten[1].y, iten[1].z)*mass*scale*scale,
								glm::vec3(iten[2].x, iten[2].y, iten[2].z)*mass*scale*scale);


		int numBodies;
		ss >> numBodies;
		info.numBodies.push_back(numBodies);


		// 0 - position
		// 1 - orienation
		// 2 - velocity
		// 3 - angular velocity
		float min[4][3];
		float max[4][3];
		int range[4][3];
		for(int a = 0; a < 4; a++)
		{
			for(int r = 0; r < 3; r++)
			{
				ss >> min[a][r];
				ss >> max[a][r];
				range[a][r] = max[a][r] - min[a][r] + 1;
			}
		}

		printf("\nPosition distribution:\n");
		printMinMax(min[0], max[0]);

		printf("\nOrientation distribution:\n");
		printMinMax(min[1], max[1]);		

		printf("\nVelocity distribution:\n");
		printMinMax(min[2], max[2]);

		printf("\nAngular Velocity distribution:\n");
		printMinMax(min[3], max[3]);

		printf("\nGenerating %d bodies\n", numBodies);

		srand(time(0));
		for(int n = 0; n < numBodies; n++)
		{
			glm::vec3 attributes[4];

			for(int a = 0; a < 4; a++)
			{
//				attributes[a].x = (float)((rand() % range[a][0]) + min[a][0]);
//				attributes[a].y = (float)((rand() % range[a][1]) + min[a][1]);
//				attributes[a].z = (float)((rand() % range[a][2]) + min[a][2]);

				attributes[a].x = randRange(min[a][0], max[a][0]);
				attributes[a].y = randRange(min[a][1], max[a][1]);
				attributes[a].z = randRange(min[a][2], max[a][2]);
			}
			//pos rot vel avel
			//glm::vec3 pos(attributes[0], attributes[0], attributes[0]);
			//glm::vec3 rot(attributes[1], attributes[1], attributes[1]);
			//glm::vec3 vel(attributes[2], attributes[2], attributes[2]);
			//glm::vec3 avel(attributes[3], attributes[3], attributes[3]);

			bodies.push_back(Body(modelIndex, textureIndex, mass, attributes[0], attributes[1], attributes[2], attributes[3], inertiaTensor));
			printf("\nBody %d added with:\n", n);
			printf("\tPosition (%f, %f, %f)\n", attributes[0].x, attributes[0].y, attributes[0].z);
			printf("\tOrientation (%f, %f, %f)\n", attributes[1].x, attributes[1].y, attributes[1].z);
			printf("\tVelocity (%f, %f, %f)\n", attributes[2].x, attributes[2].y, attributes[2].z);
			printf("\tAngular Velocity (%f, %f, %f)\n", attributes[3].x, attributes[3].y, attributes[3].z);
		}

	}

	unsigned int bSize = sizeof(Body) * bodies.size();
	printf("\nAllocating %d bytes of GPU memory\n", bSize);

	cudaMalloc((void**)&devBodies, bSize);

	printf("Copying bodies\n");

	cudaMemcpy(devBodies, bodies.data(), bSize, cudaMemcpyHostToDevice);

	printf("Copied\n");


	// temp workaround


	info.numBodies[0]=bodies.size();
	printf("%d\n", info.numBodies[0]);

	//

	display.createInstanceData(info.models[0], info.numBodies[0], info.cudaResources);

	//printf("%s\n", );

	info.loaded = true;
}


Body* Scene::getCUDABodies()
{
	return devBodies;
}

glData Scene::getCUDAResourcePointers()
{
	glData data;
	data.modelRT = NULL;
	data.color = NULL;

	cudaGraphicsMapResources(2, info.cudaResources, 0);

	size_t colBytes = info.numBodies[0] * sizeof(float) * 4;
	size_t matBytes = colBytes * 4;

	cudaGraphicsResourceGetMappedPointer((void**)&data.modelRT, &(matBytes), info.cudaResources[0]);
	cudaGraphicsResourceGetMappedPointer((void**)&data.color, &(colBytes), info.cudaResources[1]);

	return data;
}

void Scene::unmapCUDAResources()
{
	cudaGraphicsUnmapResources(2, info.cudaResources, 0);
}

void Scene::unregisterCUDA()
{
	cudaGraphicsUnregisterResource(info.cudaResources[0]);
	cudaGraphicsUnregisterResource(info.cudaResources[1]);
}

RenderInfo Scene::getRenderInfo()
{
	return info;
}

Scene::~Scene()
{
	
}
