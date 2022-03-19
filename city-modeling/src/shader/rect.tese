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


void main()
{

	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	float omu = 1 - u;
	float omv = 1 - v;
	
	vec4 yoyo = vec4(omu * omv * vec3(-i0, 0, i1) + u * omv * vec3(i0, 0, i1) + u * v * vec3(i0, 0, -i1) + omu * v * vec3(-i0, 0, -i1) + 0.0000001*gl_in[3].gl_Position.xyz, 1.0f);

	gl_Position = projection * view * model * yoyo;
	eNormal = vec3(model * yoyo);
	eFragPos = vec3(model * yoyo);
}