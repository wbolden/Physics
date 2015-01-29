#include "Physics.cuh"
#include "stdio.h"


__global__ void nBody(Body* bodies, int numBodies) //would also pass physics model
{

}

//__global__ void 
__global__ void integrate(Body* bodies, glm::mat4* models, glm::vec4* colors, int numBodies ,float timestep)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;

	if(i < numBodies)
	{
		colors[i] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

__global__ void test(glm::mat4* models, glm::vec4* colors, int numBodies)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;

	if(i < numBodies)
	{
		printf("%f, %f, %f, %f\n", colors[i].x, colors[i].y, colors[i].z, colors[i].w);
	}
}

void runPhysics(Body* bodies, glm::mat4* models, glm::vec4* colors, int numBodies, float timestep)
{
	dim3 blockSize = 512;
	dim3 gridSize = dim3((numBodies+blockSize.x-1)/blockSize.x);

//	printf("%d, %d\n", sizeof(glm::mat4), sizeof(float)*16);

//	nBody<<<gridSize, blockSize>>>(bodies);
	integrate<<<gridSize, blockSize>>>(bodies, models, colors, numBodies, timestep);
	//test<<<gridSize, blockSize>>>(models, colors, numBodies);



}

