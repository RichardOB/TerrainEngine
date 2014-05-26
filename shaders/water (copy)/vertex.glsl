#version 420

layout (location=1) in vec3 position;
layout (location=2) in vec3 normal;

layout(binding=0) uniform sampler2D tex;
//uniform vec3 lightPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

out vec4 v_colour;
out vec3 v_normal;
out vec3 reflection;
out vec3 v_lightDir;

const mat4 bias = mat4(.5f, .0f, .0f, .0f,
   .0f, .5f, .0f, .0f,
   .0f, .0f, .5f, .0f,
   .5f, .5f, .5f, 1.f);

void main()
{
   gl_Position = projection * view * world * vec4(position, 1.0f);
   //v_light = bias * lightProj * lightView * world * vec4(position, 1.0f);
   v_colour = vec4(0.0f, 0.0f, 1.0f, 1.0f);
   v_normal = normal;

   //position point in world space
   vec3 w_position = (world * vec4(position, 1.0f)).xyz;
	
   //normal vector in world space
   vec3 w_normal = (world * vec4(position, 0.0f)).xyz;

    //position of camera
    vec3 w_eye = (inverse(view) * vec4(0, 0, 0, 1)).xyz;
    //view vector
    vec3 w_view = normalize(w_position - w_eye);

reflection = reflect(w_view, w_normal);

   vec3 lightPos = vec3(1000.0f, 3000.0f, 1000.0f);
   v_lightDir = normalize(lightPos - (world * vec4(position, 1.0f)).xyz);
}

