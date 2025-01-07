#version 410
layout (location = 0) in vec2 position; //input is x and y, z will be set to zero
layout (location = 1) in vec3 color;   //input is RGB, A-component is set to 1.0 in shader
uniform float zoomFactor;  //will be set from outside
out vec4 vertexColor;   //output color to fragment shader
void main() 
{
    gl_Position = vec4(position * zoomFactor, 0.0, 1.0);
    vertexColor = vec4(color, 1.0);
}
