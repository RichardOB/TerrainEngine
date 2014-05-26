#version 420

layout (location=1) in vec3 position;
layout (location=2) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

out vec3 v_colour;
out vec2 TexCoord;


void main()
{
	vec4 light = vec4(1000, 2000, 1000, 1);
	
	gl_Position  = projection * view * world * vec4(position, 1.0f);

	v_colour = vec3(0.0f, 1.0f, 0.0f);
	TexCoord = vec2(1.0f, 1.0f);
}


