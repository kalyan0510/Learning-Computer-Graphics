// Tessellation Evaluation Shader
#version 410 core

layout (triangles, equal_spacing, cw) in;

vec4 lerp3D(vec4 v0, vec4 v1, vec4 v2)
{
    return (gl_TessCoord.x) * v0 + (gl_TessCoord.y) * v1 + (gl_TessCoord.z) * v2;
}

void main()
{
	//FragPos = lerp3D(fragpos_c[0],fragpos_c[1],fragpos_c[2]);
	//Normal = lerp3D(normals_c[0],normals_c[1],normals_c[2]);
	gl_Position = lerp3D(gl_in[0].gl_Position,gl_in[1].gl_Position,gl_in[2].gl_Position);
}
// void main()
// {
// 	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) + 
// 				  (gl_TessCoord.y * gl_in[1].gl_Position) + 
// 				  (gl_TessCoord.z * gl_in[2].gl_Position);
// }