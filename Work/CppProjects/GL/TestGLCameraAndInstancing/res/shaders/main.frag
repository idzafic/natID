//Created by IDz on 20240116
#version 410

in vec2 fragTexCoord;
in float mixingMode;
uniform sampler2D texture1;
uniform sampler2D texture2;
out vec4 fragColor;

void main() 
{
    if (mixingMode == 0) 
    {
        fragColor = texture(texture1, fragTexCoord);
    } 
    else if (mixingMode == 1) 
    {
        fragColor = texture(texture2, fragTexCoord);
    } else if (mixingMode == 2) 
    {
        // blend the colors equally
        fragColor = mix(texture(texture1, fragTexCoord), texture(texture2, fragTexCoord), 0.5); 
    }
    else
    {
        discard; //discard this fragment
    }
}