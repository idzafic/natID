#version 410 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D myTextureSampler;

void main()
{

	vec3 MaterialDiffuseColor = texture( myTextureSampler, UV).rgb;

	color =  vec4(MaterialDiffuseColor, 1.0);

}