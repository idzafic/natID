#version 450

layout (location = 0) in vec2 fragTexCoord;

layout (set = 0, binding = 1) uniform texture2D textureSampler;
layout (set = 0, binding = 2) uniform sampler samplerState;

layout (location = 0) out vec4 fragColor;

void mainFS() 
{
    fragColor = texture(sampler2D(textureSampler, samplerState), fragTexCoord);
}
