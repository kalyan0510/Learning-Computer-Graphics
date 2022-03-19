// Tessellation Control Shader
#version 410 core

layout (vertices = 4) out;
out vec3 normals_c[];

uniform vec4 innerTlevels;
uniform vec4 outerTlevels;

void main()
{
	if(gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = innerTlevels[0];
		gl_TessLevelInner[1] = innerTlevels[1];
		gl_TessLevelOuter[0] = outerTlevels[0];
		gl_TessLevelOuter[1] = outerTlevels[1];
		gl_TessLevelOuter[2] = outerTlevels[2];
		gl_TessLevelOuter[3] = outerTlevels[3];
	}

	gl_out[gl_InvocationID].gl_Position
		= gl_in[gl_InvocationID].gl_Position;
}