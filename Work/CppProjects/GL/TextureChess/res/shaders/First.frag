//Created by IDz on 20240109
#version 410
in vec2 fragTexCoord;
uniform sampler2D textureSampler;
out vec4 fragColor;

void main() 
{
    fragColor = texture(textureSampler, fragTexCoord);
}
