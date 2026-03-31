#version 450
layout(std430, set = 0, binding = 0) readonly buffer Positions { vec4 positions[]; };
layout(std430, set = 0, binding = 1) readonly buffer Colors    { vec4 colors[];    };

layout(location = 0) out vec4 vColor;

void mainVS() 
{
    uint i = uint(gl_VertexIndex);
    gl_Position = positions[i];             // your positions[] are already vec4
    vColor      = colors[i];                // pass full color (or .rgb if you prefer)
}
