#version 450

layout(set = 0, binding = 1) uniform ViewportUBO {
    vec4 extentPx;    
} viewport;


void mainVS()
{
    // This covers the entire screen
    vec2 positions[4] = vec2[](
        vec2(-1.0, -1.0),  // Bottom-left
        vec2( 1.0, -1.0),  // Bottom-right
        vec2(-1.0,  1.0),  // Top-left
        vec2( 1.0,  1.0)   // Top-right
    );
    
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}
