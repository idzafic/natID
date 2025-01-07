#version 410 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 vertexNormal;

uniform mat4 v;
uniform mat4 mv;
uniform mat4 n;
uniform mat4 mvp;
uniform vec3 lightPos;


out vec2 fragTexCoord;
out vec4 v_normal;
out vec3 FragPos;
out vec3 LightPos;


void main() 
{
    LightPos = vec3(v * vec4(lightPos, 1.0)); 
	v_normal = normalize( n * vec4(vertexNormal, 1.0));
	FragPos = vec3(mv * vec4(vertexPosition, 1.0));
	fragTexCoord = texCoord;
	gl_Position = mvp * vec4(vertexPosition, 1.0);

}


