#version 420

layout (vertices = 3) out;

uniform float tessLevel;

void main()
{
   gl_out[gl_InvocationID].gl_Position =
      gl_in[gl_InvocationID].gl_Position;

   /*gl_TessLevelOuter[0] = tessLevel;
   gl_TessLevelOuter[1] = tessLevel;
   gl_TessLevelOuter[2] = tessLevel;
   gl_TessLevelInner[0] = tessLevel;*/
	
   gl_TessLevelOuter[0] = 2.0f;
   gl_TessLevelOuter[1] = 2.0f;
   gl_TessLevelOuter[2] = 2.0f;
   gl_TessLevelInner[0] = 2.0f;
}

