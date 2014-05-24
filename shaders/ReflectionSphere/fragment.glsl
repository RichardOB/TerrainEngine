#version 430

layout(binding=0) uniform samplerCube eveningScene;
layout(binding=1) uniform samplerCube dayScene;

const int sampleWidth = 1;

const float Ka = 0.5f;
const float Kd = 2f;

uniform float mixRatio;

in vec4 v_colour;
in vec3 v_normal;
in vec4 v_light;
in vec3 v_lightDir;
in vec3 reflection;

out vec4 fragment;

void main()
{
   float Ia = Ka;
   float Id = Kd * max(dot(v_normal, v_lightDir), 0);

   //vec4 centre = vec4(v_light.x, v_light.y, v_light.z - 0.005f, v_light.w);
   float shadow = 0.0f;
   for (int x = -sampleWidth; x < sampleWidth; x++)
   {
      for (int y = -sampleWidth; y < sampleWidth; y++)
      {
         //vec4 idx = vec4(centre.x + x, centre.y + y, centre.z, centre.w);
         //shadow += textureProj(shadowMap, idx);
      }
   }
   //shadow /= pow(sampleWidth * 2 + 1, 2);

   float intensity = Ia  * Id;
	
	//vec4 col = texture(cubeMap, reflection);
	vec4 colour_1 = texture(dayScene, reflection);
	vec4 colour_2 = texture(eveningScene, reflection);
	vec4 f_colour = mix (colour_1, colour_2, mixRatio);
   
   fragment = vec4(/*intensity*/ 0.8f * f_colour);

	//vec3 col = texture(cubeMap, reflection);
}
