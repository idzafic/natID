#version 450

// Input from vertex shader
layout (location = 0) in vec2 fragTexCoord;

// Separate texture and sampler (Vulkan requirement)
layout (set = 0, binding = 2) uniform texture2D textureImage;
layout (set = 0, binding = 3) uniform sampler textureSampler;

// Output color
layout (location = 0) out vec4 fragColor;

void mainFS() 
{

    fragColor = texture(sampler2D(textureImage, textureSampler), fragTexCoord);
}
