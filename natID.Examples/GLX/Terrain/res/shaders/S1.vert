#version 450

// ---- Vertex ----
// Vertex input attributes - convert from storage buffers
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

// Uniform buffer for MVP matrix (keeping this unchanged)
layout(set = 0, binding = 3) uniform TerrainUniforms {
    mat4 mvp;
} uniforms;

layout(location = 0) out vec3 vColor;

void mainVS()
{
    gl_Position = uniforms.mvp * vec4(position, 1.0);
    vColor = color;
}