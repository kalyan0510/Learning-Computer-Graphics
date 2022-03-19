// Tessellation Control Shader
#version 410 core


layout (vertices = 3) out;

in vec3 Normal[];
in vec3 FragPos[];


out vec3 normals_c[];
out vec3 fragpos_c[];

void main()
{
	//if(gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = 3.0;
		gl_TessLevelOuter[0] = 3.0;
		gl_TessLevelOuter[1] = 3.0;
		gl_TessLevelOuter[2] = 3.0;
	}

	normals_c[gl_InvocationID] = Normal[gl_InvocationID];
	fragpos_c[gl_InvocationID] = FragPos[gl_InvocationID];
	gl_out[gl_InvocationID].gl_Position
		= gl_in[gl_InvocationID].gl_Position;
}