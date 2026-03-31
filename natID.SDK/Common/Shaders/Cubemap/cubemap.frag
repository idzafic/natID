#version 450

// Input from vertex shader
layout(location = 0) in vec3 fragTexCoord;

// Cubemap texture and sampler (separate) - using unique bindings
layout(binding = 3) uniform textureCube cubemapTexture;
layout(binding = 4) uniform sampler cubemapSampler;

// Output color
layout(location = 0) out vec4 outColor;

void mainCubeFS() {
    // Sample from cubemap using interpolated normalized direction vector
    outColor = texture(samplerCube(cubemapTexture, cubemapSampler), fragTexCoord);
}
