#version 450 core

// Input from vertex shader
layout (location = 0) in vec2 fragTexCoord;
layout (location = 1) flat in uint mixingMode;

// Separate textures and  sampler
layout (set = 0, binding = 2) uniform texture2D texture1;
layout (set = 0, binding = 3) uniform texture2D texture2;
layout (set = 0, binding = 4) uniform sampler textureSampler;

// Output
layout (location = 0) out vec4 fragColor;

void mainFS() 
{
    if (mixingMode == 0) 
    {
        fragColor = texture(sampler2D(texture1, textureSampler), fragTexCoord);
    } 
    else if (mixingMode == 1) 
    {
        fragColor = texture(sampler2D(texture2, textureSampler), fragTexCoord);
    } else if (mixingMode == 2) 
    {
        // blend the colors equally
        fragColor = mix(texture(sampler2D(texture1, textureSampler), fragTexCoord), texture(sampler2D(texture2, textureSampler), fragTexCoord), 0.5); 
    }
    else
    {
        discard; //discard this fragment
    }
}