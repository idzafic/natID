#version 450

// Vertex input attributes (binding 0 reserved for vertex input)
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

// Uniform buffer object for matrices and scalars
layout (set = 0, binding = 4) uniform UniformBufferObject {
    mat4 mvp; // model-view-projection matrix
    vec4 textureScale; // only paramater x is used
} ubo;

// Output to fragment shader
layout (location = 0) out vec2 fragTexCoord;

void mainVS() 
{
    gl_Position = ubo.mvp * vec4(position, 0.0, 1.0);
    fragTexCoord = texCoord * ubo.textureScale.x;
}

