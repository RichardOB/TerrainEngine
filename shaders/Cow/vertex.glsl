#version 420

layout (location=1) in vec3 position;
layout (location=2) in vec3 colour;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

out vec3 v_colour;

   
out vec3 n;
out vec3 l;

void main()
{
	
	
	//Position of light in world space
	vec4 light = vec4(1000, 2000, 1000, 1);
	
	gl_Position = projection * view * world * vec4(position, 1.0f);
	
	vec3 pos = normalize(position);
	vec4 worldPos = world * vec4(pos, 1.0f);
	n = worldPos.xyz;

	//v_colour = pickColour(position.y);
	v_colour = vec3(0.54f, 0.27f, 0.07f);
	
	//find the direction vector to the light source (normalize it since we only need direction)
	l = normalize(light - worldPos).xyz;
	
}


