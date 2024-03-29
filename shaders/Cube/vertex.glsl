#version 420

layout (location=1) in vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

//out vec3 v_colour;
out vec3 TexCoord;


void main()
{
	vec4 light = vec4(1000, 2000, 1000, 1);
	
	vec4 pos = projection * view * world * vec4(position, 1.0f);
	gl_Position = pos.xyww;

	//v_colour = vec3(1.0f, 0.0f, 0.0f);
	TexCoord = position;
}


