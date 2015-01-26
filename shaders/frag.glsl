#version 420

in vec2 texCoords;
layout (binding = 0) uniform sampler2D tex;
//uniform sampler2D tex;

out vec4 fragColor;





void main()
{

	vec4 texel = texture(tex, texCoords);
	vec4 col = vec4(1, 1, 1, 1);
	//float ac = texel.w;
	fragColor = texel * col;

	//fragColor.w = 1.0;



	//fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}