#version 430

layout(binding=0) uniform samplerCube eveningScene;
layout(binding=1) uniform samplerCube dayScene;

//in vec3 v_colour;
in vec3 TexCoord;
in float ratio;

out vec4 f_colour;

void main()
{
	
	//f_colour = vec4(v_colour, 1.0f);
	
	vec4 colour_1 = texture(dayScene, TexCoord);
	vec4 colour_2 = texture(eveningScene, TexCoord);
	
	f_colour = mix (colour_1, colour_2, ratio);
}
