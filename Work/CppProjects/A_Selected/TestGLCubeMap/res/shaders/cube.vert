//Created by IDz on 20240116
#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 initialAngle; //instanced data pos 0
layout (location = 2) in vec3 translation; //instanced data pos 1
layout (location = 3) in vec3 rotationDirection; //instanced data pos 2 //-1, 0, 1 values

uniform mat4 cameraWithProjection;
uniform float accAngle;

out vec3 fragTexCoord;
out float mixingMode;

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

void main() 
{
  fragTexCoord = vec3(position.x, position.y, position.z);
  float divisor = 3.0;
  mixingMode = mod(gl_InstanceID, divisor);
  divisor = 7.0; //remove every 7th cube
  float toRemove = mod(gl_InstanceID, divisor);
  mixingMode=mixingMode+3*int(toRemove==0);
  
  // Apply rotation
  vec3 angle = initialAngle + accAngle  * rotationDirection;
  mat4 rotation = rotateZ(angle.x) * rotateY(angle.y) * rotateX(angle.z);
  //make model transformation
  vec4 rotatedAndTranslatedPosition = rotation * vec4(position, 1.0) + vec4(translation, 0.0);
  //apply full transformation
  gl_Position = cameraWithProjection * rotatedAndTranslatedPosition;
}

