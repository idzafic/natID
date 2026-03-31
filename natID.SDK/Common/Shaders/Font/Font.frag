#version 450

layout (location = 0) in vec2 fragTexCoord;
layout (location = 1) in vec4 fragColor;

layout (set = 0, binding = 5) uniform texture2D textureSampler;
layout (set = 0, binding = 6) uniform sampler samplerState;

layout (location = 0) out vec4 outColor;

void mainFS() 
{
    vec4 texColor = texture(sampler2D(textureSampler, samplerState), fragTexCoord);
    outColor = vec4(fragColor.xyz, texColor.a);


}