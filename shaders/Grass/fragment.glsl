#version 430

layout(location=1) uniform sampler2D tex;

in vec2 TexCoord;
in vec3 v_colour;

out vec4 f_colour;

void main()
{
	vec4 test = texture(tex, vec2(0.5f, 0.5f));
	//f_colour = vec4(v_colour, 1.0f);
	f_colour = vec4(mix(test.xyz, v_colour, 0.0f), 1.0f);
	//f_colour = texture (cubeMap, TexCoord);
}

