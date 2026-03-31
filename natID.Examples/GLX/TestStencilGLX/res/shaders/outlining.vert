#version 450 core

// Vertex inputs (binding 0 reserved for vertex data)
layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec3 vertexNormal;

// Uniform buffer for matrices (set = 1, binding = 1) - different set for outline pass
layout (set = 0, binding = 7) uniform OutlineMatrixUBO {
    mat4 m;
    mat4 v;
    mat4 p;
} matrices;

#define SCALEMODEL

void mainVS_outline()
{

#ifdef SCALEMODEL
    gl_Position = matrices.p * matrices.v * matrices.m * vec4(vertexPosition, 1.0f);
#else
    float outlineThickness = 0.03;
    gl_Position = matrices.p * matrices.v * matrices.m * vec4(vertexPosition + vertexNormal * outlineThickness, 1.0f);
#endif

}