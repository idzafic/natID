//Created by IDz on 20240116
#version 410

in vec2 fragTexCoord;
in float mixingMode;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 fragColor;

void main() 
{
	if (mixingMode > 2.0) {discard;} //Trebalo bi moci bez ove linije
	float a=min(1.0, mixingMode)/max(1.0, mixingMode);
    fragColor = mix(texture(texture1, fragTexCoord), texture(texture2, fragTexCoord), a);
	
	//Za alpha=0.0 dobije se bijela kocka umjesto prozirne
	//fragColor.a = min(1.0, (3.0-mixingMode));
}