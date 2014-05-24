#version 430

//Our fragment shader needs to do tha actual "sampling".
//We need to add a Texture variable for the texture first.
//Location of Cube Map set explicitly - not possible in shader version 4.2 so we use 4.3
layout(location=0) uniform samplerCube cubeMap;
uniform float shininess;

in vec3 reflection;

out vec4 colour;

void main()
{
/*
   // Ambient light intensity.
   float ambient = 0.1f;

   // Diffuse light intensity.
   float diffuse = dot(t_normal, t_lightDir);

   // Specular light intensity. The reflect() function below takes 2 arguments:
   // I and N. I is an /incident/ vector --- that is, the direction the light
   // is coming from. Our light vector is the direction /towards/ the light
   // source. This means we need to flip it around. N is the normal, which 
   // works as normal.
   vec3 reflect = reflect(-t_lightDir, t_normal);
   float specular = pow(dot(reflect, t_viewDir), shininess);

   // Ensure all light intensity values are positive, then add them to obtain
   // the final intensity value.
   float intensity = ambient;
   intensity += diffuse > 0 ? diffuse : 0;
   intensity += specular > 0 ? specular : 0;
 
   // Multiply final intensity value with colour and set alpha value to 1.
   colour = vec4(t_colour * intensity, 1.0);
*/
	//The actual "sampling"
	//colour = texture(cubeMap, reflection);
	colour = vec4 (1.0f, 0.0f, 0.0f, 1.0f);

}