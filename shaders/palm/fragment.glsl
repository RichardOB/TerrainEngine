#version 430

layout(binding=2) uniform sampler2D tex;

const int sampleWidth = 1;

const float Ka = 0.5f;
const float Kd = 2f;

uniform float gamma;

in vec4 v_colour;
in vec3 v_normal;
in vec4 v_light;
in vec3 v_lightDir;
in vec2 TexCoord;
in float distance;

out vec4 fragment;

void main()
{
	
	const vec4 fog_colour = vec4(0.7, 0.7, 0.7, 1.0);
	const float min_fog_radius = 10.0;
	const float max_fog_radius = 100.0;
	
	//Get a fog factor based on distance
	float fog_fac = (distance - min_fog_radius) / (max_fog_radius - min_fog_radius);
	//Constrain the fog factor between 0 and 1
	fog_fac = clamp (fog_fac, 0.0, 1.0);
	
	float Ia = Ka;
	float Id = Kd * max(dot(v_normal, v_lightDir), 0);

	
	vec4 col = texture(tex, TexCoord);
	float intensity = Ia /*+ shadow*/ * Id;
	//fragment = vec4(intensity * col.xyz, 1.0f);
	vec4 f_col = vec4(intensity * col.xyz, 1.0f);
	
	if (gamma == 1.0f)
	{
		fragment = sqrt(mix(f_col, fog_colour, fog_fac));
	}
	else
	{
		fragment = mix(f_col, fog_colour, fog_fac);
	}
	
}
