#version 420

layout (binding = 0) uniform sampler2D tex;

in vec2 texCoords;
in vec4 color;

out vec4 fragColor;



void main()
{
	vec4 texel = texture(tex, texCoords);

	fragColor = texel * color;//clamp(color, vec4(0, 0, 0, 0), vec4(1, 1, 1, 1));
}