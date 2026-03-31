#version 450

layout(location = 0) in vec3 vertexPosition;

layout(set = 0, binding = 8) uniform DepthPassUBO {
    mat4 depthMVP;
} ubo;

void mainDepthVS(){
	gl_Position = ubo.depthMVP * vec4(vertexPosition,1);
}

