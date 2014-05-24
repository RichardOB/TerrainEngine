#version 420

layout (location=1) in vec3 position;
layout (location=2) in vec3 colour;
//layout (location=2) in vec3 normal;

//layout(binding=0) uniform sampler2D tex;
//uniform vec3 lightPos;

//uniform mat4 lightProj;
//uniform mat4 lightView;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

out vec3 v_colour;
//out vec3 v_normal;
//out vec4 v_light;
//out vec3 v_lightDir;

/*const mat4 bias = mat4(.5f, .0f, .0f, .0f,
   .0f, .5f, .0f, .0f,
   .0f, .0f, .5f, .0f,
   .5f, .5f, .5f, 1.f);*/
   
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
	
	/*
	vec3 pos = normalize(position);
	vec4 worldPos = world * vec4(pos, 1.0f);
	n = worldPos.xyz;

	v_colour = pickColour(position.y);
	
	//Position of light in world space
	vec4 light = vec4(1024, 0, 1024, 1);
	
	//find the direction vector to the light source (normalize it since we only need direction)
	l = normalize(light - worldPos).xyz;
	
	gl_Position = projection * view * world * vec4(position, 1.0f);
	
	*/
}


