#ifndef BODY_H
#define BODY_H
#include "cuda.h"
//#include "cuda.h"
//#include "cuda_runtime.h"
#include "vector_types.h" //CUDA vectors

#define GLM_FORCE_CUDA
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Body
{
	unsigned int modelId;
	unsigned int texId;

	glm::vec3 position;
	glm::quat orienation;

	glm::vec3 force;
	glm::vec3 torque;

	float mass;
	glm::vec3 aMomentum;
	glm::vec3 lMomentum;

	glm::mat3 iTensor;
	glm::mat3 invITensor;


	Body(unsigned int mId, unsigned int tId, float m, glm::vec3 pos, glm::vec3 rot, glm::vec3 vel, glm::vec3 avel, glm::mat3 inertiaTensor)
	{
		modelId = mId;
		texId = tId;

		mass = m;
		position = pos;
		//rot

		lMomentum = mass * vel;
		aMomentum = inertiaTensor * avel;

		iTensor = inertiaTensor;
		invITensor = glm::inverse(inertiaTensor);

		force = glm::vec3(0, 0, 0);
		torque = glm::vec3(0, 0, 0);;
	}
};

#endif