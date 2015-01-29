#version 420

in vec2 texCoords;
in vec4 mcolor;
layout (binding = 0) uniform sampler2D tex;
//uniform sampler2D tex;

out vec4 fragColor;

//in int inst;



void main()
{

	vec4 texel = texture(tex, texCoords);
	vec4 col = vec4(1, 1, 1, 1);
	//float ac = texel.w;
	col = mcolor;

	//if((gl_InstanceID % 3) == 0)
	//{
	//	col = mcolor;
	//}

	fragColor = texel * col;

	//fragColor.w = 1.0;



	//fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}