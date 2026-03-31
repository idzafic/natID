#version 450

layout(location = 0) flat in vec3 color;

layout(location = 0) out vec4 frag_color;

void mainFSflat() 
{
    frag_color = vec4(color, 1.0);
}
