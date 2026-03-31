#version 450

// Input from vertex shader
layout(location = 0) in vec4 vertexColor;

// Output color
layout(location = 0) out vec4 fragColor;

void mainFS() 
{
    // Output the interpolated vertex color
    fragColor = vertexColor;
}
