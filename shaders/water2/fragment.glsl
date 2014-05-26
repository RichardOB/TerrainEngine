#version 430

layout(binding=3) uniform sampler2D tex;



const int sampleWidth = 1;

const float Ka = 1.0f;
const float Kd = 2f;

in vec4 v_colour;
in vec3 v_normal;
in vec4 v_light;
in vec3 v_lightDir;
in vec3 reflection;
in vec2 texCoords;

out vec4 fragment;

void main()
{
   float Ia = Ka;
   float Id = Kd * max(dot(v_normal, v_lightDir), 0);

   float intensity = Ia  * Id;
	
	//vec4 col = texture(cubeMap, reflection);
	vec4 col = vec4(0.0f, 0.0f, 1.0f, 0.2f);
	vec4 col2 = texture(tex,  texCoords);
	
	if (col2.a < 0.2f)
	{
		discard;
	}
	
	fragment = col2;

	//vec3 col = texture(cubeMap, reflection);
}
