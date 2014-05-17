#version 420

layout(location=0) uniform samplerCube cubeMap;

in vec3 TexCoord;

out vec4 f_colour;

void main()
{
	
	//f_colour = vec4(v_colour, 1.0f);
	f_colour = texture (cubeMap, TexCoord);
}

