#version 410 core

// INPUT (iz C++ koda)
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;

// UNIFORMS (iz C++ koda)
uniform mat4 u_mvp;
uniform mat4 u_mv;
uniform mat4 u_n;
uniform vec3 u_cameraPos; // <--- OVO JE NEDOSTAJALO

// OUTPUT (za Fragment Shader)
out vec3 v_normal;
out vec2 v_texCoord;
out float v_height;
out float viewSpaceDistance;
out vec3 v_worldPos;      // <--- NOVO
out vec3 v_viewVec;       // <--- NOVO

void main()
{
    gl_Position = u_mvp * vec4(a_position, 1.0);

    vec4 viewSpacePos = u_mv * vec4(a_position, 1.0);
    viewSpaceDistance = length(viewSpacePos.xyz);

    v_normal = normalize((u_n * vec4(a_normal, 0.0)).xyz);

    v_texCoord = a_texCoord;
    v_height = a_position.y;

    // NOVI DIO:
    v_worldPos = a_position;
    v_viewVec = u_cameraPos - v_worldPos; // Sada u_cameraPos postoji
}