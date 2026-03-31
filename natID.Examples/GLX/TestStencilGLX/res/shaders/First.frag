#version 450 core

const float gamma = 2.2;

// Inputs from vertex shader
layout (location = 0) in vec2 fragTexCoord;
layout (location = 1) in vec4 v_normal;
layout (location = 2) in vec3 FragPos;
layout (location = 3) in vec3 LightPos;
layout (location = 4) in vec3 v_tangent;   // Add tangent vector
layout (location = 5) in vec3 v_bitangent; // Add bitangent vector

// Separate texture and sampler (set = 0, binding = 3 and 4)
layout (set = 0, binding = 3) uniform texture2D texWoodFloor;
layout (set = 0, binding = 4) uniform sampler texWoodFloorSampler;

// Normal map texture and sampler (set = 0, binding = 5 and 6)
layout (set = 0, binding = 5) uniform texture2D texWoodFloorNorm;
layout (set = 0, binding = 6) uniform sampler texWoodFloorNormSampler;

// Uniform buffer for lighting (set = 0, binding = 8) - shared with vertex shader
layout (set = 0, binding = 2) uniform LightingUBO {
    vec3 lightPos;
    vec3 lightColor;
} lighting;

// Output
layout (location = 0) out vec4 fragColor;

vec3 phong(vec3 modelColor, vec3 normal)
{    
    vec3 LightPosition = LightPos;

    float mKa = 0.1, mKd = 1.0, mKs = 0.4;
    vec3 Ka = lighting.lightColor; // Ambient 
    vec3 Kd = lighting.lightColor; // Diffuse 
    vec3 Ks = lighting.lightColor; // Specular 
    float Shininess = 128;

    vec3 n = normalize(normal);
    vec3 s = normalize(LightPosition - FragPos);
    vec3 v = normalize(vec3(-FragPos));
    vec3 r = reflect(-s, n);

    return
    modelColor * 
    ( 
    mKa * Ka +
    mKd * Kd * max(dot(s, n), 0.0) +
    mKs * Ks * pow(max(dot(r,v), 0.0), Shininess)
    );
}

vec3 getNormalFromMap()
{
    // Sample the normal map
    vec3 normalMap = texture(sampler2D(texWoodFloorNorm, texWoodFloorNormSampler), fragTexCoord).rgb;
    
    // Convert from [0,1] to [-1,1] range
    normalMap = normalMap * 2.0 - 1.0;
    
    // Create TBN matrix (Tangent, Bitangent, Normal)
    vec3 N = normalize(v_normal.xyz);
    vec3 T = normalize(v_tangent);
    vec3 B = normalize(v_bitangent);
    
    // Ensure orthogonality (Gram-Schmidt process)
    T = normalize(T - dot(T, N) * N);
    B = cross(N, T);
    
    mat3 TBN = mat3(T, B, N);
    
    // Transform normal from tangent space to world space
    return normalize(TBN * normalMap);
}

void mainFS()
{
    // Use separate texture and sampler
    vec4 color1 = texture(sampler2D(texWoodFloor, texWoodFloorSampler), fragTexCoord);
    color1.rgb = pow(color1.rgb, vec3(gamma));

    // Get the normal from the normal map
    vec3 finalNormal = getNormalFromMap();

    fragColor = vec4(phong(color1.xyz, finalNormal), color1.w);

    // apply gamma correction
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}

