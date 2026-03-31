#version 450 core

// Vertex inputs (binding 0 reserved for vertex data)
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 vertexNormal;

// Uniform buffer for matrices (set = 0, binding = 1)
layout (set = 0, binding = 1) uniform MatrixUBO {
    mat4 v;
    mat4 mv;
    mat4 n;
    mat4 mvp;
} matrices;

// Uniform buffer for lighting (set = 0, binding = 2)
layout (set = 0, binding = 2) uniform LightingUBO {
    vec3 lightPos;
    vec3 lightColor;
} lighting;

// Outputs to fragment shader
layout (location = 0) out vec2 fragTexCoord;
layout (location = 1) out vec4 v_normal;
layout (location = 2) out vec3 FragPos;
layout (location = 3) out vec3 LightPos;
layout (location = 4) out vec3 v_tangent;   // Add tangent output
layout (location = 5) out vec3 v_bitangent; // Add bitangent output

void mainVS() 
{
    LightPos = vec3(matrices.v * vec4(lighting.lightPos, 1.0)); 
    v_normal = normalize(matrices.n * vec4(vertexNormal, 1.0));
    FragPos = vec3(matrices.mv * vec4(vertexPosition, 1.0));
    fragTexCoord = texCoord;
    
    // Calculate tangent and bitangent vectors for cube faces
    vec3 normal = normalize(vertexNormal);
    vec3 tangent, bitangent;
    
    // For cube faces, we can determine tangent based on normal direction
    if (abs(normal.x) > 0.9) {
        // X-faces (left/right)
        tangent = vec3(0.0, 0.0, sign(normal.x));
        bitangent = vec3(0.0, 1.0, 0.0);
    } else if (abs(normal.y) > 0.9) {
        // Y-faces (top/bottom) 
        tangent = vec3(1.0, 0.0, 0.0);
        bitangent = vec3(0.0, 0.0, -sign(normal.y));
    } else {
        // Z-faces (front/back)
        tangent = vec3(-sign(normal.z), 0.0, 0.0);
        bitangent = vec3(0.0, 1.0, 0.0);
    }
    
    // Transform tangent and bitangent to view space
    v_tangent = vec3(matrices.n * vec4(tangent, 0.0));
    v_bitangent = vec3(matrices.n * vec4(bitangent, 0.0));
    
    gl_Position = matrices.mvp * vec4(vertexPosition, 1.0);
}


