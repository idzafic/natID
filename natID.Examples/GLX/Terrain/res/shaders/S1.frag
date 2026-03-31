#version 450

// ---- Fragment ----
layout(location = 0) in vec3 vColor;

layout(location = 0) out vec4 fragColor;

void mainFS()
{
    fragColor = vec4(vColor, 1.0);
}
