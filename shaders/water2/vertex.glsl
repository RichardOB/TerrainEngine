#version 420

layout (location=1) in vec3 position;
layout (location=3) in vec2 uv;

//layout(binding=0) uniform sampler2D tex;
//uniform vec3 lightPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

uniform float fog;

out vec3 v_normal;
out vec3 v_lightDir;
out vec2 texCoords;
out float distance;

const mat4 bias = mat4(.5f, .0f, .0f, .0f,
   .0f, .5f, .0f, .0f,
   .0f, .0f, .5f, .0f,
   .5f, .5f, .5f, 1.f);

void main()
{
	gl_Position = projection * view * world * vec4(position, 1.0f);

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

	//Calculate Light values
	vec3 lightPos = vec3(100.0f, 300.0f, 100.0f);
	v_lightDir = normalize(lightPos - (world * vec4(position, 1.0f)).xyz);
	
	texCoords = uv;
}

