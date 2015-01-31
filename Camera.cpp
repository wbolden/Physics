#include "Camera.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define ONED 0.01745329f

/*
	move direction set on key press, removed on key up
	new key presses invalidate previous ones
	if a key press was invalidated key up does nothing 

*/

Camera::Camera()
{
	currentWS = 0;
	currentAD = 0;
	currentUD = 0;
	currentRL = 0;
	currentBoost = 1;

	pos = glm::vec3(0.0f, 0.0f, 3.0f);

	rot = glm::vec3(0.0f, 3.14159f, 0.0f);


	proj = glm::perspective(70.0f, 800.0f/600.0f, 0.1f, 1000000.0f);
	moveSpeed = 10;
	rotSpeed = 3.14;
}

Camera::Camera(float fov, float width, float height, float near, float far)
{
	currentWS = 0;
	currentAD = 0;
	currentUD = 0;
	currentRL = 0;
	currentBoost = 1;

	pos = glm::vec3(0.0f, 0.0f, 3.0f);
	rot = glm::vec3(0.0f, 3.14159f, 0.0f);
	//rot = glm::vec3(720.0f*ONED, 360.0f*ONED, 360.0f*ONED);
	proj = glm::perspective(fov, width/height, near, far);

	moveSpeed = 10;
	rotSpeed = 3.14;
}


float Camera::getMoveSpeed()
{
	return moveSpeed;
}


void Camera::update(float timestep)
{
	moveForeward(currentWS*moveSpeed*timestep*currentBoost);
	moveRight(currentAD*moveSpeed*timestep*currentBoost);

	rot.x += currentRL*rotSpeed*timestep;
	rot.y += currentUD*rotSpeed*timestep;
	//rotateUp(currentUD*moveSpeed*timestep);
	//rotateRight(currentRL*moveSpeed*timestep);
}

void Camera::setWS(float ws)
{
	currentWS = ws;
}

void Camera::setAD(float ad)
{
	currentAD = ad;
}

void Camera::setUD(float ud)
{
	currentUD = ud;
}

void Camera::setRL(float rl)
{
	currentRL = rl;
}

void Camera::setBoost(float boost)
{
	currentBoost = boost;
}



#include <iostream>
void Camera::moveForeward(float ammount)
{
	pos.x += ammount*sinf(rot.x)*cosf(rot.y);  //-
	pos.y += -ammount*sinf(rot.y);  //-
	pos.z += ammount*cosf(rot.x)*cosf(rot.y); //+
}


void Camera::moveRight(float ammount)
{
	pos.x += ammount*cosf(rot.x); //+
	pos.z += -ammount*sinf(rot.x); //+
}

void Camera::rotateRight(float ammount)
{
	rot.x += ammount;
	//printf("rotx: %f\n", rot.x/ONED);
}

void Camera::rotateUp(float ammount)
{
	rot.y += ammount;
	//printf("roty: %f\n", rot.y/ONED);
}

glm::mat4 Camera::getViewMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::rotate(view, rot.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, rot.x, glm::vec3(0.0f, -1.0f, 0.0f));

	view = glm::translate(view, pos);

	//printf("pos: (%f, %f, %f)\n",pos.x, pos.y, pos.z);

	return view;
}

glm::mat4 Camera::getProjMatrix()
{
	return proj;
}

Camera::~Camera()
{

}