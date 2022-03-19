// Tessellation Evaluation Shader
#version 410 core

layout (triangles, equal_spacing) in;

vec3 bezier(float u, vec3 p0, vec3 p1, vec3 p2, vec3 p3)
{
	float bf0 = (1.-u)*(1.-u)*(1.-u);
	float bf1 = 3.*u*(1.-u)*(1.-u);
	float bf2 = 3.*u*u*(1.-u);
	float bf3 = u*u*u;

	vec3 p = bf0*p0 + bf1*p1 + bf2*p2 + bf3*p3;
	return p;
}

void main()
{

	float u = gl_TessCoord.x;

	vec3 p0 = vec3( gl_in[0].gl_Position );
	vec3 p1 = vec3( gl_in[1].gl_Position );
	vec3 p2 = vec3( gl_in[2].gl_Position );
	vec3 p3 = vec3( gl_in[3].gl_Position );
	//gl_Position = vec4( p0*u+(1-u)*p3 ,1.0);
	gl_Position = vec4( bezier( u, p0, p1, p2, p3), 1. );
}
