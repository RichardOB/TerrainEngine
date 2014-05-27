#version 420

//layout(binding=6) uniform sampler2DShadow shadowMap;

//const int sampleWidth = 1;

//const float Ka = 0.3f;
//const float Kd = 2f;
uniform float gamma;

in vec3 v_colour;

in vec3 n;
in vec3 l;
//in vec3 v_normal;
//in vec4 v_light;
//in vec3 v_lightDir;
in float distance;

//out vec4 fragment;
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
	
	float ambientCoefficient = 0.1f;
	float diffuseCoefficient = 0.8f;
	
	float ambient = ambientCoefficient;
	float diffuse = diffuseCoefficient * max(dot(l, n), 0);
	
	float intensity = ambient + diffuse;
	
	vec4  f_col = vec4(v_colour * intensity, 1.0f); //with alpha value added. For now it is left to completely opaque.
	
	if (gamma == 1.0f)
	{
		fragment = sqrt(mix(f_col, fog_colour, fog_fac));
	}
	else
	{
		fragment = mix(f_col, fog_colour, fog_fac);
	}
	

}
