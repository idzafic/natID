#version 450

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;


layout (set = 0, binding = 3) uniform UniformBufferObject {
    mat4 mvp;              // 64 bytes - model-view-projection matrix
    vec4 params;           // 16 bytes - params.x = textureScale, other components unused
} ubo;

layout (location = 0) out vec2 fragTexCoord;

void mainVS() 
{
    gl_Position = ubo.mvp * vec4(position, 0.0, 1.0);
    fragTexCoord = texCoord * ubo.params.x; // params.x contains textureScale
}

