#version 410

layout(triangles) in;
layout (triangle_strip, max_vertices=3) out;


in vec3 eFragPos[3];
in vec3 eNormal[3];

out vec3 FragPos;
out vec3 Normal;

uniform int mode;

 void main()
 {
        vec3 n = cross(eFragPos[1].xyz-eFragPos[0].xyz, eFragPos[2].xyz-eFragPos[0].xyz);
        //vec3 n = eNormal[0] + eNormal[1] + eNormal[2];
        for(int i = 0; i < gl_in.length(); i++)
        {
             gl_Position = gl_in[i].gl_Position;

             Normal = mode==3?eFragPos[i]:n;
             FragPos = eFragPos[i];

             EmitVertex();
        }
}