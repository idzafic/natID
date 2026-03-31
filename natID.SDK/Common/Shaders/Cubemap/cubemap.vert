#version 450

// Vertex input
layout(location = 0) in vec3 inPosition;

// Uniforms at binding 1 (slot 1)
// viewProjection should have translation removed: projection * mat4(mat3(view))
layout(binding = 1) uniform UniformBufferObject {
    mat4 viewProjection;
} ubo;

// Output to fragment shader
layout(location = 0) out vec3 fragTexCoord;

void mainCubeVS() {
    // Normalize position to create consistent direction for cubemap lookup
    fragTexCoord = normalize(inPosition);
    
    // Transform to clip space (no scaling or translation needed)
    vec4 clipPos = ubo.viewProjection * vec4(inPosition, 1.0);
    
    // Set z to w for max depth (skybox rendered at far plane)
    gl_Position = clipPos.xyww;
}
