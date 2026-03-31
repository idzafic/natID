//Created by IDz on 20240116
#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vertex_color;

uniform mat4 mvp;

out vec3 color;

void main() 
{
  gl_Position = mvp * vec4(pos, 1.0);
  color = vertex_color;
}

