#version 410 core

layout (location = 0) in vec2 aPos;


void main()
{
    float w = 1600;
    float h = 900;
    gl_Position = vec4((2*aPos.x)/w - 1.0, (2*aPos.y)/h - 1.0, 0.0, 1.0);
}
