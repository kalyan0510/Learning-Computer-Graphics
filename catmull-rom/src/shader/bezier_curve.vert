#version 410 core

layout (location=0) in vec2 apos;

void
main()
{
	gl_Position = vec4(apos.x, apos.y, 0.0, 1.0);
}
