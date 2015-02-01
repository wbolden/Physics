#include "Physics.cuh"
//#include "Math.cuh"
#include "stdio.h"

#define G 6.67e-11f
#define k 7.0e0f  //7.0e-1f
#define da 10.0f  //0.9f

#define k2 1.0e2f


__global__ void nBody(Body* bodies, int numBodies, float ks) //would also pass physics model
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if(i < numBodies)
	{
		bodies[i].force = glm::vec3(0,0,0);
		glm::vec3 fGravity(0,0,0);

		for(int n = 0; n < numBodies; n++)
		{
			if(n != i)
			{
				glm::vec3 rin = bodies[n].position - bodies[i].position; 

				float dotr = glm::dot(rin, rin);
				float distr = sqrtf(dotr);

				glm::vec3 dirvec = rin/distr;

				if(distr < 2)
				fGravity += bodies[n].mass * dirvec*0.25f;
				else
				fGravity += bodies[n].mass * dirvec/ (distr*distr);

				
				if(distr < 2)
				{
					glm::vec3 colp = bodies[i].position + 0.5f*rin;

					glm::vec3 colforce = -ks* (2 - distr)*dirvec;

					glm::vec3 aveli = bodies[i].invITensor * bodies[i].aMomentum;
					glm::vec3 aveln = bodies[n].invITensor * bodies[n].aMomentum;

					glm::vec3 veli = bodies[i].lMomentum/bodies[i].mass;
					glm::vec3 veln = bodies[n].lMomentum/bodies[n].mass;

					glm::vec3 relPosCM = 0.5f * rin; //reltive position of collision to center of mass of i

					glm::vec3 tveli(0, 0, 0);
					/*
					tveli.x += aveli.x*relPosCM.x;
					tveli.y += aveli.y*relPosCM.y;
					tveli.z += aveli.z*relPosCM.z;
					*/
					tveli += veli;
					

					// - for radius
					glm::vec3 tveln(0, 0, 0);
					/*
					tveln.x += -aveln.x*relPosCM.x;
					tveln.y += -aveln.y*relPosCM.y;
					tveln.z += -aveln.z*relPosCM.z;
					*/
					tveln += veln;
					

					glm::vec3 tvel= tveli - tveln;

					glm::vec3 shearforce = k * tvel;

					glm::vec3 damp = tvel * -da;
					//glm::vec3 damp = tvel * -0.7f;

					bodies[i].force += (colforce + shearforce + damp);


				//	bodies[i].torque += glm::cross(relPosCM, (colforce + shearforce + damp));
				}
			}
		}

		fGravity = fGravity * G * bodies[i].mass;
		bodies[i].force += fGravity;
		
	}
}

//__global__ void 
__global__ void integrate(Body* bodies, glm::mat4* models, glm::vec4* colors, int numBodies ,float timestep, float boost)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;

	if(i < numBodies)
	{
		//bodies[i].force += 0.0001;

		/*
			should i include damping?
		*/


		/*
			translation
		*/
		bodies[i].lMomentum += bodies[i].force * timestep;
		glm::vec3 vel = bodies[i].lMomentum/bodies[i].mass;
		bodies[i].position += vel * timestep;

		/*
			Rotation
		*/
		//update aMom
		bodies[i].aMomentum += bodies[i].torque * timestep;

		//Update orientation

		glm::vec3 avel = bodies[i].invITensor * bodies[i].aMomentum;
		avel *= timestep;
		
		float angle = glm::length(avel);

		if(angle > 0)
		{
			float sinal = sinf(angle/2.0f)/angle;
			glm::quat inter(cosf(angle/2.0f) ,glm::vec3(avel.x*sinal, avel.y*sinal, avel.z*sinal));
			bodies[i].orienation = inter * bodies[i].orienation;
		}

		glm::mat4 modelMat = glm::mat4_cast(bodies[i].orienation);

		modelMat[3][0] += bodies[i].position.x;
		modelMat[3][1] += bodies[i].position.y;
		modelMat[3][2] += bodies[i].position.z;

		models[i] = modelMat;


		/*
			Coloring
		*/
		colors[i] = glm::vec4(bodies[i].force* 10.5f/boost, 1.0f) + glm::vec4(0.3f, 0.3f, 0.3f, 0.0f);
	}
}

__global__ void test(Body* bodies, glm::vec4* colors, int numBodies)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;

	if(i < numBodies)
	{
		//printf("%f, %f, %f, %f\n", colors[i].x, colors[i].y, colors[i].z, colors[i].w);

		printf("%f, %f, %f\n", bodies[i].force.x, bodies[i].force.y, bodies[i].force.z);
	}
}



void runPhysics(Body* bodies, glm::mat4* models, glm::vec4* colors, int numBodies, float timestep, float boost)
{
	dim3 blockSize = 512;
	dim3 gridSize = dim3((numBodies+blockSize.x-1)/blockSize.x);

//	printf("%d, %d\n", sizeof(glm::mat4), sizeof(float)*16);


	nBody<<<gridSize, blockSize>>>(bodies, numBodies, k * boost);

	integrate<<<gridSize, blockSize>>>(bodies, models, colors, numBodies, timestep, boost);
	//test<<<gridSize, blockSize>>>(bodies, colors, numBodies);


	cudaDeviceSynchronize();
}

