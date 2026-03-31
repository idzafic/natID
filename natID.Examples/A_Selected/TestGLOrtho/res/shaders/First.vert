//Created by IDz on 20240109
#version 410 core

layout (location = 0) in vec2 position;
uniform vec3 color;
uniform mat4 mvp; //model-view-projection matrix

out vec3 fragColor;

void main() 
{
    gl_Position = mvp * vec4(position, 0.0, 1.0);
    fragColor = color;
}

