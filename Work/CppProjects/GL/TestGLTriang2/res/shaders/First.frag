//Created by IDz on 20240109
#version 410
in vec3 fragColor; //as defined in vertex shared
out vec4 color;

void main() 
{
    color = vec4(fragColor, 1.0);
}
