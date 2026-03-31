#version 450

// Vertex input attributes (bound from vertex descriptor)
layout(location = 0) in vec2 position;  // Vertex position from buffer index 0
layout(location = 1) in vec3 color;     // Vertex color from buffer index 0

// Uniform buffer (bound from buffer index 1)
layout(set = 0, binding = 1) uniform SimpleUniforms {
    float zoomFactor;
    float pad0;
    float pad1;
    float pad2;
} uniforms;

// Output to fragment shader
layout(location = 0) out vec4 vertexColor;

void mainVS() 
{
    // Apply zoom factor and convert 2D position to 4D clip space
    gl_Position = vec4(position * uniforms.zoomFactor, 0.0, 1.0);
    
    // Pass color to fragment shader with full alpha
    vertexColor = vec4(color, 1.0);
}
