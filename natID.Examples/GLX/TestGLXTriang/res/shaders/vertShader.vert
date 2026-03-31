#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTranslation;
layout(location = 3) in float inScale;

layout(std140, set = 0, binding = 6) uniform UBO {
    mat4 projection;
};

layout(location = 0) out vec4 vColor;

void mainVS() 
{
    vec2 worldPos = inPosition * inScale + inTranslation;
    gl_Position = projection * vec4(worldPos, 0.0, 1.0);
    vColor = inColor;
}
