#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 FragPosLightSpace;

uniform mat4 perspective;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    vec3 FragPos = vec3(model * vec4(aPos, 1.0));
    mat4 shadowMat = lightSpaceMatrix * model;
 
    FragPosLightSpace = shadowMat * vec4(aPos, 1.0);
    gl_Position = perspective * view * model * vec4(aPos, 1.0);
}