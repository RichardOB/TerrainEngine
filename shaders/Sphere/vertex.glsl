#version 420

layout (location=1) in vec3 position;
layout (location=2) in vec3 normal;

layout(binding=0) uniform sampler2D tex;
uniform vec3 lightPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

out vec4 v_colour;
out vec3 v_normal;
;
out vec3 v_lightDir;

const mat4 bias = mat4(.5f, .0f, .0f, .0f,
   .0f, .5f, .0f, .0f,
   .0f, .0f, .5f, .0f,
   .5f, .5f, .5f, 1.f);

void main()
{
   gl_Position = projection * view * world * vec4(position, 1.0f);
   //v_light = bias * lightProj * lightView * world * vec4(position, 1.0f);
   v_colour = vec4(0.6f, 0.6f, 0.6f, 1.0f);
   v_normal = normal;

   //vec3 lightPos = vec3(10.0f, 10.0f, 10.0f);
   v_lightDir = normalize(lightPos - (world * vec4(position, 1.0f)).xyz);
}

