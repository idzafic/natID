#version 450 core

// Vertex attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 translation; //instanced data pos 0
layout (location = 3) in vec3 instanceColor; //instanced data pos 1

// Uniform buffer object
layout (set = 0, binding = 6) uniform UniformBufferObject {
    float accAngle;
    float _pad0;
    float _pad1;
    float _pad2;
    vec4 cameraEye;
    mat4 cameraWithProjection;
} ubo;

// Output to fragment shader
layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec3 fragNormal;
layout (location = 2) out vec3 fragWorldPos;
layout (location = 3) out vec3 fragCameraEye;

void mainVS() 
{
    fragColor = instanceColor;
    fragNormal = normal;
    fragCameraEye = ubo.cameraEye.xyz;

    vec3 worldPosition = position + translation;
    fragWorldPos = worldPosition;
    gl_Position = ubo.cameraWithProjection * vec4(worldPosition, 1.0);
}
