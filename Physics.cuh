#ifndef PHYSICS_CUH
#define PHYSICS_CUH

#include "Body.h"
#include "cuda.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <GL/gl.h>
#include <GL/glu.h>

#define GLM_FORCE_CUDA
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void runPhysics(Body* bodies, glm::mat4* models, glm::vec4* colors, int numBodies, float timestep);


#endif