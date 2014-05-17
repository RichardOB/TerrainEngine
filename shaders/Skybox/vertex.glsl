#version 420

layout (location=1) in vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

//out vec3 v_colour;
out vec3 TexCoord;



void main()
{
	vec4 pos = projection * view * world * vec4(position, 1.0f);
	//Get w for z coordinate so that it is 1.o after Perspective Divide (Leads to Infinite Depth)
	gl_Position = pos.xyww;

	TexCoord = position;
}


