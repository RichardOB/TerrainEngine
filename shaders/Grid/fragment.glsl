#version 420

//layout(binding=6) uniform sampler2DShadow shadowMap;

//const int sampleWidth = 1;

//const float Ka = 0.3f;
//const float Kd = 2f;

in vec3 v_colour;

in vec3 n;
in vec3 l;
//in vec3 v_normal;
//in vec4 v_light;
//in vec3 v_lightDir;

//out vec4 fragment;
out vec4 f_colour;

void main()
{
	//Position of static camera
	//vec3 view = normalize(vec3(1024,2000,1024));
	
	float ambientCoefficient = 0.0f;
	float diffuseCoefficient = 0.8f;
	
	float ambient = ambientCoefficient;
	float diffuse = diffuseCoefficient * max(dot(l, n), 0);
	
	float intensity = ambient + diffuse;
	
	f_colour = vec4(v_colour * intensity, 1.0f); //with alpha value added. For now it is left to completely opaque.
	
	
	/*
	//Position of static camera
	vec3 view = normalize(vec3(1024,2000,1024));
	
	float ambientCoefficient = 0.0f;
	
	float diffuse = dot(l, n);
	
	vec3 r = 2 * dot(l, n) * n - l;
	float specular = pow(dot(r, view), 10);
	
	float intensity = ambientCoefficient;
	
	//we do not wan't to add negative values of diffuse or specular to intensity
	if (diffuse > 0)
	{
		intensity += diffuse;
	}
	
	if (specular > 0)
	{
		intensity += specular;
	}
	
	f_colour = vec4(v_colour * intensity, 1.0f); //with alpha value added. For now it is left to completely opaque.
	*/
	

}
