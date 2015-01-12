#ifndef BODY_H
#define BODY_H

#include "cuda.h"
#include "cuda_runtime.h"

struct Model
{
	//gluint texid;
	//gluint vao;
	//	--vao contains vertices vbo, normals vbo, and texcoords vbo
};

struct Body
{
	float3 r;
	//float3 a;
	float3 F;
	float3 t;
	float3 p;
	float3 L;
	//Quaternion q;
	//Mat R;
	float m;
	unsigned int modelId;

	float3 position;
	float3 force;
	float3 torque;
	float3 linearMomentum;
	float3 angularMomentum;
	//Quaternion rotation;
	//Mat rotation;
	float mass;


};

#endif