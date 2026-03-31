#version 450 core

// Vertex attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 initialAngle; //instanced data pos 0
layout (location = 3) in vec3 translation; //instanced data pos 1
layout (location = 4) in vec3 rotationDirection; //instanced data pos 2 //-1, 0, 1 values

// Uniform buffer object
layout (set = 0, binding = 6) uniform UniformBufferObject {
    float accAngle;
    mat4 cameraWithProjection;
} ubo;

// Output to fragment shader
layout (location = 0) out vec2 fragTexCoord;
layout (location = 1) flat out uint mixingMode;

mat4 rotateX(float angle) 
{
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);
    return mat4(1.0, 0.0, 0.0, 0.0,
                0.0, cosTheta, -sinTheta, 0.0,
                0.0, sinTheta, cosTheta, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

mat4 rotateY(float angle) 
{
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);
    return mat4(cosTheta, 0.0, sinTheta, 0.0,
                0.0, 1.0, 0.0, 0.0,
                -sinTheta, 0.0, cosTheta, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

mat4 rotateZ(float angle) 
{
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);
    return mat4(cosTheta, -sinTheta, 0.0, 0.0,
                sinTheta, cosTheta, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

void mainVS() 
{
    fragTexCoord = texCoord;
    int divisor = 3;
    mixingMode = uint(mod(gl_InstanceIndex, divisor));
    divisor = 7; //remove every 7th cube
    float toRemove = mod(gl_InstanceIndex, divisor);
    if (toRemove == 1)
    {
        gl_Position = vec4(0.0); // Or any value outside the view frustum
        mixingMode = 5;
        return;
    }
    // Apply rotation
    vec3 angle = initialAngle + ubo.accAngle * rotationDirection;
    mat4 rotation = rotateZ(angle.x) * rotateY(angle.y) * rotateX(angle.z);
    //make model transformation
    vec4 rotatedAndTranslatedPosition = rotation * vec4(position, 1.0) + vec4(translation, 0.0);
    //apply full transformation
    gl_Position = ubo.cameraWithProjection * rotatedAndTranslatedPosition;
}
