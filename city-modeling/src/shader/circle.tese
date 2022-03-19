// Tessellation Evaluation Shader
#version 410 core
#define PI 3.14

layout (quads, equal_spacing, cw) in;

out vec3 eFragPos;
out vec3 eNormal;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

uniform float i0;//a
uniform float i1;//b
uniform float i2;//r
uniform float i3;//ts
uniform float i4;//te
uniform float i5;//rs
uniform float i6;//re

vec4 lerp3D(vec4 v0, vec4 v1, vec4 v2)
{
    return (gl_TessCoord.x) * v0 + (gl_TessCoord.y) * v1 + (gl_TessCoord.z) * v2;
}

void main()
{

	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	float r = u;
	float th = v*(i4-i3) + i3;
	float omu = 1 - u;
	float omv = 1 - v;
	
	vec4 x = omu * omv * gl_in[0].gl_Position + u * omv * gl_in[1].gl_Position + u * v * gl_in[2].gl_Position +
	omu * v * gl_in[3].gl_Position;
	vec4 y = vec4(
	r*cos(th)*i0,
	i2,
	r*sin(th)*i1,
	1.0f) + vec4(.00001f)*x;
	gl_Position = projection * view * model * (y);
	eNormal = vec3(model * y);
	eFragPos = vec3(model * y);
}