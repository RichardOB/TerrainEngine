#version 420

layout (location=1) in vec3 position;
layout (location=2) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

uniform float fog;

out vec3 v_colour;
out vec2 TexCoord;
out vec3 v_normal;
out float distance;
out vec3 v_lightDir;

void main()
{
	vec4 light = vec4(1000, 2000, 1000, 1);
	
	gl_Position  = projection * view * world * vec4(position, 1.0f);
	
	//Calculate normal for phong lighting (just y axis since grass is vertical)
	v_normal = vec3(0.0f, 1.0f, 0.0f);
	
	//Calculate distance for distance fog
	vec4 surface_pos_eye =  view * world * vec4(position, 1.0f);
	distance = length(-surface_pos_eye);
	
	//Disable/Enable Distance Fog
	if (fog == 0.0f)
	{
		distance = 0.0f;
	}

	v_colour = vec3(0.0f, 1.0f, 0.0f);
	TexCoord = vec2(1.0f, 1.0f);
	
	//Calculate Light values
	vec3 lightPos = vec3(0.0f, 20.0f, 0.0f);
	v_lightDir = normalize(lightPos - (world * vec4(position, 1.0f)).xyz);
}


