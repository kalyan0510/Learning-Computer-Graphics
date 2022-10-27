#version 410 core


in vec3 Normal;
in vec3 FragPos;


out vec4 FragColor;


uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;


void main()
{
    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    FragColor = vec4(abs(norm), 1.0);
}
