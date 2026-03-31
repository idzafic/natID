#version 450

// Input from vertex shader
layout (location = 0) in vec2 fragTexCoord;

// Separate texture and sampler bindings (required for Vulkan backend)
layout (binding = 3) uniform texture2D textureImage;
layout (binding = 4) uniform sampler textureSampler;


// Output color
layout (location = 0) out vec4 fragColor;

void mainFS() 
{
    fragColor = texture(sampler2D(textureImage, textureSampler), fragTexCoord);
}