#version 410 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;


out vec3 FragPos;
out vec3 Normal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    vec4 temp =  vec4(aPos, 1.0);
    FragPos = vec3(temp.x/temp.w,temp.y/temp.w,temp.z/temp.w);
    Normal = mat3(transpose(inverse(model))) * aNorm;
    gl_Position = vec4(FragPos, 1.0);
}