// Tessellation Evaluation Shader
#version 410 core

layout (triangles, equal_spacing) in;

vec3 crfun(float u, vec3 p0, vec3 p1, vec3 p2, vec3 p3)
{
	float B0 = -u*u*u*0.5 + u*u - 0.5*u;
	float B1 = 1.5*u*u*u + (.5-3.)*u*u + 1.;
	float B2 = (.5-2)*u*u*u + 2.*u*u + .5*u;
	float B3 = .5*u*u*u - .5*u*u;

	vec3 p = B0*p0 + B1*p1 + B2*p2 + B3*p3;
	return p;
}

void main()
{
	float u = gl_TessCoord.x;

	vec3 p0 = vec3( gl_in[0].gl_Position );
	vec3 p1 = vec3( gl_in[1].gl_Position );
	vec3 p2 = vec3( gl_in[2].gl_Position );
	vec3 p3 = vec3( gl_in[3].gl_Position );
	gl_Position = vec4( crfun( u, p0, p1, p2, p3), 1. );
}
