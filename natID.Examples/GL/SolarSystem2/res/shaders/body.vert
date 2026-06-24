#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aTexCoord;
uniform mat4 mvp;
out vec2 texCoord;
void main() {
    gl_Position = mvp * vec4(position, 1.0);
    texCoord = aTexCoord;
}
