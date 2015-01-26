#ifndef CAMERA_H
#define CAMERA_H

//#include "Input.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera
{
public:
	Camera();
	Camera(float fov, float width, float height, float near, float far);

	void setRotation(/*Some stuff*/);
	void setPosition(/*Some stuff*/);

	float getMoveSpeed();
	glm::mat4 getViewMatrix();

	glm::mat4 getProjMatrix();

	void moveForeward(float ammount);
	void moveRight(float ammount);

	void update(float timestep);

	void setWS(float ws);
	void setAD(float ad);

	void setUD(float ud);
	void setRL(float rl);

	void setBoost(float boost);


	void rotateRight(float ammount);
	void rotateUp(float ammount);

	~Camera();

private:
	//float x, y, z;
	//float xt, yt, zt;
	float moveSpeed;
	float rotSpeed;

	glm::mat4 proj;
	glm::vec3 pos;
	glm::vec3 rot;

	float currentWS;
	float currentAD;

	float currentUD;
	float currentRL;

	float currentBoost;


	//Quaternion rot;

};

#endif