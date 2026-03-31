#version 450

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 vertexNormal;

layout (binding = 2) uniform UniformBufferObject {
    mat4 v;
    mat4 mv;
    mat4 n;
    mat4 mvp;
    vec4 lightPos; // Changed from vec3 to vec4 for proper alignment
} ubo;

layout (location = 0) out vec2 fragTexCoord;
layout (location = 1) out vec4 v_normal;
layout (location = 2) out vec3 FragPos;
layout (location = 3) out vec3 LightPos;

void mainVS() 
{
    LightPos = vec3(ubo.v * vec4(ubo.lightPos.xyz, 1.0)); 
    v_normal = normalize(ubo.n * vec4(vertexNormal, 1.0));
    FragPos = vec3(ubo.mv * vec4(vertexPosition, 1.0));
    fragTexCoord = texCoord;
    gl_Position = ubo.mvp * vec4(vertexPosition, 1.0);
}



