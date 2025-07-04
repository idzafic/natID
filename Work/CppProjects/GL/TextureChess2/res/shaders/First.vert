//Created by IDz on 20250517
#version 410 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
uniform mat4 mvp; //model-view-projection matrix
out vec2 fragTexCoord;

void main() 
{
    gl_Position = mvp * vec4(position, 0.0, 1.0);
    fragTexCoord = texCoord;
}

