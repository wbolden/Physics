#ifndef BODY_H
#define BODY_H

//#include "cuda.h"
//#include "cuda_runtime.h"
#include "vector_types.h" //CUDA vectors

struct Body
{
	unsigned int modelId;
	unsigned int texId;
	float mass;

	float3 position;
	float3 force;
	float3 torque;
	float3 linearMomentum;
	float3 angularMomentum;

	//must add com, proper amom, scale
	Body(unsigned int mid, unsigned int tid, float m, float3 pos, float3 rot, float3 v, float3 w)
	{
		modelId = mid;
		texId = tid;
		mass = m;
		position = pos;
	//	rot;


		/*
			Must be calculated from mass/ itensor
		*/

		linearMomentum = v;
		angularMomentum = w;
	}
};

#endif