#version 410
in vec4 vertexColor;  //input color from vertex shader (linearly scaled between vertices)
out vec4 fragColor;  //output color 
void main() 
{
    fragColor = vertexColor;
}
