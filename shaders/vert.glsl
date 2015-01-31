#version 420

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

layout(location = 3) in mat4 modelRT; //Ma
layout(location = 7) in vec4 modelColor;



out vec2 texCoords;
out vec4 color;



uniform mat4 proj;
uniform mat4 view;

//uniform mat4 mtest;

void main()
{
	texCoords = texCoord;
	color = modelColor;

	//vec4 pos = vec4(gl_InstanceID*2, 0, 0, 1.0f) + vec4(vertex, 0.0f);


	vec4 position = modelRT * vec4(vertex, 1.0f);

	//pos.x += gl_InstanceID*2;

	//position.x += gl_InstanceID*2;


	gl_Position =  proj * view * position;

	//gl_Position = proj * view * vec4(position, 1.0f);

	//gl_Position = proj * view * model * vec4(position, 1.0f);


	//gl_Position = vec4(vp.x + pos.x, vp.y + pos.y, 0, 1.0) * scale;
}