#version 450

// Input vertex attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

// Uniform buffer for MVP matrix
layout (binding = 1, std140) uniform CubeUniforms
{
    mat4 mvp;
};

// Output to fragment shader
layout (location = 0) out vec2 fragTexCoord;

void mainVS() 
{
    gl_Position = mvp * vec4(position, 1.0);
    fragTexCoord = texCoord;
}
