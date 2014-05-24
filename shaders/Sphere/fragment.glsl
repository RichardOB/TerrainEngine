#version 420

layout(binding=6) uniform sampler2DShadow shadowMap;

const int sampleWidth = 1;

const float Ka = 0.3f;
const float Kd = 2f;

in vec4 v_colour;
in vec3 v_normal;
in vec4 v_light;
in vec3 v_lightDir;

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

   float intensity = Ia /*+ shadow*/ * Id;
   fragment = vec4(intensity * v_colour.xyz, 1.0f);
}
