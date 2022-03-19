// Tessellation Evaluation Shader
#version 410 core
#define PI 3.14

layout (quads, equal_spacing, cw) in;

out vec3 eFragPos;
out vec3 eNormal;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

uniform float i0;
uniform float i1;
uniform float i2;
uniform float i3;
uniform float i4;
uniform float i5;
uniform float i6;
uniform float i7;
uniform float i8;

vec4 lerp3D(vec4 v0, vec4 v1, vec4 v2)
{
    return (gl_TessCoord.x) * v0 + (gl_TessCoord.y) * v1 + (gl_TessCoord.z) * v2;
}

void main()
{

	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	float phi = u*(i6-i5) + i5;
	float th = v*(i4-i3) + i3;
	float omu = 1 - u;
	float omv = 1 - v;
	
	vec4 x = omu * omv * gl_in[0].gl_Position + u * omv * gl_in[1].gl_Position + u * v * gl_in[2].gl_Position +
	omu * v * gl_in[3].gl_Position;
	vec4 y = vec4(
	i0*sign(cos(phi))*sign(cos(th))*pow(abs(cos(phi)), i7)*pow(abs(cos(th)), i8),
	i1*sign(cos(phi))*sign(sin(th))*pow(abs(cos(phi)), i7)*pow(abs(sin(th)), i8),
	i2*sign(sin(phi))*pow(abs(sin(phi)), i7),
	1.0f) + vec4(.00001f)*x;
	gl_Position = projection * view * model * (y);
	eNormal = vec3(model * y);
	eFragPos = vec3(model * y);
}