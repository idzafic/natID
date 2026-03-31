#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vertex_color;

layout(set = 0, binding = 2) uniform UniformBufferObject {
    mat4 mvp;
} ubo;

layout(location = 0) flat out vec3 color;

void mainVSflat() 
{
    gl_Position = ubo.mvp * vec4(pos, 1.0);
    color = vertex_color;
}

