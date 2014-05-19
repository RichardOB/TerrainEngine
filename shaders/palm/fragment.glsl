#version 430

layout(location=1) uniform sampler2D tex;

const int sampleWidth = 1;

const float Ka = 0.8f;
const float Kd = 2f;

in vec4 v_colour;
in vec3 v_normal;
in vec4 v_light;
in vec3 v_lightDir;
in vec2 TexCoord;

out vec4 fragment;

void main()
{
   float Ia = Ka;
   float Id = Kd * max(dot(v_normal, v_lightDir), 0);

	
   vec4 col = texture(tex, TexCoord);
   float intensity = Ia /*+ shadow*/ * Id;
   fragment = vec4(intensity * col.xyz, 1.0f);
}
