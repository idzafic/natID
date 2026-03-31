#version 330 core

in vec2 UV;
in vec4 ShadowCoord;

layout(location = 0) out vec3 color;

uniform sampler2D myTextureSampler;
uniform sampler2DShadow shadowMap;

void main(){

	vec3 LightColor = vec3(1,1,1);
	
	vec3 MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;

	float visibility = texture( shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z-0.001)/ShadowCoord.w) );
	visibility = clamp(visibility, 0.1, 1.0);

	color = visibility * MaterialDiffuseColor * LightColor;

}