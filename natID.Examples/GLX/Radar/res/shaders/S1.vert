#version 450

// Use storage buffer for vertex positions instead of hardcoded array
layout(std430, set = 0, binding = 3) readonly buffer Positions { 
    vec4 positions[]; 
};

void mainVS() 
{
    // Use vertex index to access position from storage buffer
    gl_Position = vec4(positions[gl_VertexIndex]);
}

