#version 420

layout (location=1) in vec3 position;
layout (location=2) in vec3 normals;
layout (location=3) in vec2 uv;

layout(binding=0) uniform sampler2D tex;
//uniform vec3 lightPos;

uniform mat4 lightProj;
uniform mat4 lightView;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
uniform vec3 lightPos;

uniform float fog;

out vec4 v_colour;
out vec3 v_normal;
//out vec4 v_light;
out vec3 v_lightDir;
out vec2 TexCoord;

out float distance;

const mat4 bias = mat4(.5f, .0f, .0f, .0f,
   .0f, .5f, .0f, .0f,
   .0f, .0f, .5f, .0f,
   .5f, .5f, .5f, 1.f);

void main()
{
	gl_Position = projection * view * world * vec4(position, 1.0f);
	//v_light = bias * lightProj * lightView * world * vec4(position, 1.0f);
	v_colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	v_normal = normals;
	
	vec4 surface_pos_eye =  view * world * vec4(position, 1.0f);
	distance = length(-surface_pos_eye);
	
	if (fog == 0.0f)
	{
		distance = 0.0f;
	}
 

	TexCoord = uv;

	v_lightDir = normalize(lightPos - (world * vec4(position, 1.0f)).xyz);
}

