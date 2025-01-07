#version 410 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec3 vertexNormal;


uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

#define SCALEMODEL

void main()
{

#ifdef SCALEMODEL
	gl_Position = p * v * m * vec4(vertexPosition, 1.0f);
#else
	float outlineThickness = 0.03;
	gl_Position = p * v * m * vec4(vertexPosition + vertexNormal * outlineThickness, 1.0f);
#endif

}