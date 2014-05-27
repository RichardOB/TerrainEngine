#version 430

layout(location=1) uniform sampler2D tex;

in vec2 TexCoord;
in vec3 v_colour;
in vec3 v_normal;
in vec3 v_lightDir;
in float distance;

const float Ka = 0.8f;
const float Kd = 2f;

out vec4 fragment;

void main()
{
	//Fog distance, thickness and colour values
	const vec4 fog_colour = vec4(0.7, 0.7, 0.7, 1.0);
	const float min_fog_radius = 10.0;
	const float max_fog_radius = 100.0;
	
	//Get a fog factor based on distance
	float fog_fac = (distance - min_fog_radius) / (max_fog_radius - min_fog_radius);
	//Constrain the fog factor between 0 and 1
	fog_fac = clamp (fog_fac, 0.0, 1.0);
	
	//Lighting coefficients
	float Ia = Ka;
	float Id = Kd * max(dot(v_normal, v_lightDir), 0);
	
	float intensity = Ia /*+ shadow*/ * Id;
	
	vec4 colour = texture(tex, vec2(0.5f, 0.5f));
	
	vec4 f_col = vec4(0.0f * colour.xyz, 1.0f);

	//f_colour = vec4(mix(colour.xyz, v_colour, 0.0f), 1.0f);
	
	fragment = mix(f_col, fog_colour, fog_fac);

}

