#version 410 core

// INPUT
in vec3 v_normal;
in vec2 v_texCoord;
in float v_height;
in float viewSpaceDistance;
in vec3 v_worldPos;
in vec3 v_viewVec;

// Uniforms
uniform vec3 u_lightDir;
uniform vec3 u_sunColor;
uniform float u_time;

// Textures
uniform sampler2D u_texSand;
uniform sampler2D u_texLightGrass;
uniform sampler2D u_texGrass;
uniform sampler2D u_texDarkGrass;
uniform sampler2D u_texStonyGrass;
uniform sampler2D u_texRock;
uniform sampler2D u_texSnow;

// Fog
uniform vec3 fogColor;
uniform float fogStart;
uniform float fogEnd;

out vec4 FragColor;

// Water
const mat2 rot = mat2(0.8, 0.6, -0.6, 0.8);

float hash( vec2 p ) {
	float h = dot(p,vec2(127.1,311.7));	
    return fract(sin(h)*43758.5453123);
}

float noise( in vec2 p ) {
    vec2 i = floor( p );
    vec2 f = fract( p );	
	vec2 u = f*f*(3.0-2.0*f);
    return -1.0+2.0*mix( mix( hash( i + vec2(0.0,0.0) ), hash( i + vec2(1.0,0.0) ), u.x),
                        mix( hash( i + vec2(0.0,1.0) ), hash( i + vec2(1.0,1.0) ), u.x), u.y);
}

float fbm ( vec2 p ) {
    float f = 0.0;
    f += 0.5000 * noise(p); p = rot * p * 2.02;
    f += 0.2500 * noise(p); p = rot * p * 2.03;
    f += 0.1250 * noise(p);
    return f / 0.875;
}


void main()
{
    
    // Terrain
    float terrain_diffuse = max(dot(normalize(v_normal), -normalize(u_lightDir)), 0.0);
    vec3 terrain_light = u_sunColor * terrain_diffuse + vec3(0.2);
    vec4 sandTex = texture(u_texSand, v_texCoord);
    vec4 lightGrassTex = texture(u_texLightGrass, v_texCoord);
    vec4 grassTex = texture(u_texGrass, v_texCoord);
    vec4 stonyGrassTex = texture(u_texStonyGrass, v_texCoord);
    vec4 rockTex = texture(u_texRock, v_texCoord);
    vec4 snowTex = texture(u_texSnow, v_texCoord);
    vec4 terrain_color_sampler = sandTex;
    terrain_color_sampler = mix(terrain_color_sampler, lightGrassTex, smoothstep(2.5, 5.0, v_height));
    terrain_color_sampler = mix(terrain_color_sampler, grassTex,      smoothstep(4.5, 9.0, v_height));
    terrain_color_sampler = mix(terrain_color_sampler, stonyGrassTex, smoothstep(7.0, 12.0, v_height));
    terrain_color_sampler = mix(terrain_color_sampler, rockTex,       smoothstep(11.0, 16.0, v_height));
    terrain_color_sampler = mix(terrain_color_sampler, snowTex,       smoothstep(18.0, 20.0, v_height));
    vec3 final_terrain_color = terrain_color_sampler.rgb * terrain_light;

    // Water
    vec2 uv = v_worldPos.xz;
    float time = u_time * 0.01; // Slowed down movement

    // Layers of waves
    // Big waves
    vec2 uv_large = uv * 0.1;
    float large_waves = fbm(uv_large + time * 0.8);

    // Medium waves
    vec2 uv_medium = uv * 0.4;
    float medium_ripples = fbm(uv_medium - time * 1.0);

    // Smaller details
    vec2 uv_small = uv * 1.5;
    float small_details = fbm(uv_small + time * 1.2);
    
    // Combines all layers to get the final shape of area
    float displacement = large_waves * 0.6 + medium_ripples * 0.25 + small_details * 0.15;
    
    // Calculate the normal based on the combined waves
    vec3 water_normal = normalize(vec3(
        displacement - fbm((uv_large + vec2(0.01, 0.0)) + time*0.5)*0.6,
        0.1, // Sharpness of the waves, a larger number = smoother water
        displacement - fbm((uv_large + vec2(0.0, 0.01)) + time*0.5)*0.6
    ));

    vec3 viewDir = normalize(v_viewVec);
    vec3 lightDir = -normalize(u_lightDir);

    // Contrast and lighting
    float fresnel = 0.02 + 0.98 * pow(1.0 - dot(vec3(0.0, 1.0, 0.0), viewDir), 5.0);
    float specular = pow(max(dot(reflect(-lightDir, water_normal), viewDir), 0.0), 20.0); // Softer, larger highlight
    float diffuse = max(dot(water_normal, lightDir), 0.0);

    vec3 deep_color = vec3(0.0, 0.07, 0.1); // Slightly lighter dark color
    vec3 shallow_color = vec3(0.1, 0.3, 0.35); // Less saturated light color
    vec3 water_color = mix(shallow_color, deep_color, fresnel);
    
    water_color += diffuse * u_sunColor * 0.25; // Reduced influence of diffuse light
    water_color += specular * u_sunColor * 0.09; // Reduced highlight intensity, but uses the sun's color

    // Final color blending
    vec3 final_color = mix(water_color, final_terrain_color, smoothstep(2.0, 3.5, v_height));

    // Fog
    float fogFactor = smoothstep(fogStart, fogEnd, viewSpaceDistance);
    vec3 foggedColor = mix(final_color, fogColor, fogFactor);
    FragColor = vec4(foggedColor, 1.0);
}