#version 450

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec4 color;

layout (set = 0, binding = 3) uniform FontUniforms {
    mat4 mvp;
} ubo;

layout (location = 0) out vec2 fragTexCoord;
layout (location = 1) out vec4 fragColor;

void mainVS() 
{
    gl_Position = ubo.mvp * vec4(position, 0.0, 1.0);
    fragTexCoord = texCoord;
    fragColor = color;
}