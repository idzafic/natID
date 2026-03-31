#version 450 core

// Input from vertex shader
layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragNormal;
layout (location = 2) in vec3 fragWorldPos;
layout (location = 3) in vec3 fragCameraEye;

// Output
layout (location = 0) out vec4 outColor;

// Light properties
const vec3 lightDir = normalize(vec3(1.0, 1.0, 0.5));
const vec3 lightAmbient = vec3(0.2, 0.2, 0.2);
const vec3 lightDiffuse = vec3(0.7, 0.7, 0.7);
const vec3 lightSpecular = vec3(1.0, 1.0, 1.0);

// Material properties
const vec3 materialSpecular = vec3(0.5, 0.5, 0.5);
const float materialShininess = 16.0;

void mainFS() 
{
    // Use instance color as material ambient/diffuse color
    vec3 materialColor = fragColor;

    // Guard against zero normals at Extrusion endpoints
    float normLen = length(fragNormal);
    if (normLen < 0.001)
    {
        outColor = vec4(materialColor * lightAmbient, 1.0);
        return;
    }
    vec3 norm = fragNormal / normLen;

    // Ambient
    vec3 ambient = lightAmbient * materialColor;

    // Diffuse (directional light - consistent at all distances)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * (diff * materialColor);

    // Specular (Phong)
    vec3 viewDir = normalize(fragCameraEye - fragWorldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = lightSpecular * (spec * materialSpecular);

    vec3 result = ambient + diffuse + specular;
    outColor = vec4(result, 1.0);
}