#version 450

const float gamma = 2.2;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec4 v_normal;
layout(location = 2) in vec3 FragPos;
layout(location = 3) in vec3 LightPos;
layout(location = 4) in vec4 ShadowCoord;


layout(set = 0, binding = 3) uniform texture2D texWoodFloor;
layout(set = 0, binding = 4) uniform sampler texWoodFloorSampler;
layout(set = 0, binding = 5) uniform texture2D shadowMapTexture;
layout(set = 0, binding = 6) uniform samplerShadow shadowMapSampler;

// Uniform buffer for lighting parameters - properly aligned
layout(set = 0, binding = 7) uniform LightingUBO {
    vec4 lightColor;
} lighting;

layout(location = 0) out vec4 fragColor;

vec3 phong(vec3 modelColor)
{    
    vec3 LightPosition = LightPos;

    float mKa = 0.1, mKd = 1.0, mKs = 0.4;
    vec3 Ka = lighting.lightColor.xyz; 
    vec3 Kd = lighting.lightColor.xyz; 
    vec3 Ks = lighting.lightColor.xyz; 
    float Shininess = 128;

    vec3 n = normalize( v_normal.xyz );
    vec3 s = normalize( LightPosition - FragPos );
    vec3 v = normalize(vec3(-FragPos));
    vec3 r = reflect( -s, n );

    return
    modelColor * 
    ( 
    mKa * Ka +
    mKd * Kd * max( dot(s, n), 0.0 ) +
    mKs * Ks * pow( max( dot(r,v), 0.0 ), Shininess )
    );
}

void mainFS()
{
    vec4 color1 = texture(sampler2D(texWoodFloor, texWoodFloorSampler), fragTexCoord);
    color1.rgb = pow(color1.rgb, vec3(gamma));
    
    vec3 projCoords = ShadowCoord.xyz / ShadowCoord.w;
    
    // Outside shadow map bounds - assume not in shadow
    if (projCoords.x < 0.0 || projCoords.x > 1.0 || 
        projCoords.y < 0.0 || projCoords.y > 1.0 ||
        projCoords.z < 0.0 || projCoords.z > 1.0) {
        fragColor = vec4(phong(color1.rgb), 1.0);
        fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
        fragColor.a = 1.0;
        return;
    }
    
    // Calculate slope-scale bias
    vec3 normal = normalize(v_normal.xyz);
    vec3 lightDir = normalize(LightPos - FragPos);
    float cosTheta = clamp(dot(normal, lightDir), 0.0, 1.0);
    
    float bias = max(0.0001 * (1.0 - cosTheta), 0.0001);

    float visibility = texture(sampler2DShadow(shadowMapTexture, shadowMapSampler), 
                               vec3(projCoords.xy, projCoords.z - bias));
    
    visibility = clamp(visibility, 0.15, 1.0);
    
    fragColor = visibility * vec4(phong(color1.rgb), 1.0);

    fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
    fragColor.a = 1.0;
}

