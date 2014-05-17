#version 420

layout (location=1) in vec3 position;
layout (location=2) in vec3 normal;

layout(binding=0) uniform sampler2D tex;
uniform vec3 lightPos;

uniform mat4 lightProj;
uniform mat4 lightView;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

out vec4 v_colour;
out vec3 v_normal;
out vec4 v_light;
out vec3 v_lightDir;

const mat4 bias = mat4(.5f, .0f, .0f, .0f,
   .0f, .5f, .0f, .0f,
   .0f, .0f, .5f, .0f,
   .5f, .5f, .5f, 1.f);

vec4 pickColour(float height)
{
   if (height <= 50)
   {
      float idx = (height / 50.0f) * 0.25;
      return texture(tex, vec2(idx, 0.25));
   }
   else if (height <= 75)
   {
      float idx = ((height - 50.0f) / 25.0f) * 0.25f + 0.25f;
      return texture(tex, vec2(idx, 0.25));
   }
   else if (height <= 150)
   {
      float idx = ((height - 75.0f) / 75.0f) * 0.25f + 0.5f;
      return texture(tex, vec2(idx, 0.75));
   }
   else
   {
      float idx = ((height - 150.0f) / 100.0f) * 0.25f + 0.75f;
      return texture(tex, vec2(idx, 0.75));
   }
}

void main()
{
   gl_Position = projection * view * world * vec4(position, 1.0f);
   v_light = bias * lightProj * lightView * world * vec4(position, 1.0f);
   v_colour = pickColour(position.y);
   v_normal = normal;
   v_lightDir = normalize(lightPos - (world * vec4(position, 1.0f)).xyz);
}

