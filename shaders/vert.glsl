#version 420

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

//layout(location = 3) in mat4 modelRT; //Ma
//layout(location = 7) in vec4 color;



out vec2 texCoords;
//out vec3 normal
//out vec4 color

//uniform vec2 pos;
//uniform mat4 proj;
//uniform mat4 view;


uniform mat4 proj;
uniform mat4 view;

uniform mat4 mtest;

void main()
{
	texCoords = texCoord;
					//view * model * position
	//vec3 pos = position;

	vec4 pos = mtest* vec4(position, 1.0f);

	pos.x += gl_InstanceID*2;

	gl_Position = proj * view * pos;

	//gl_Position = proj * view * vec4(position, 1.0f);

	//gl_Position = proj * view * model * vec4(position, 1.0f);


	//gl_Position = vec4(vp.x + pos.x, vp.y + pos.y, 0, 1.0) * scale;
}