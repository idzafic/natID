#version 330 core
out vec4 FragColor;

in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;


void main()
{           
    FragColor = vec4(1, 1, 1, 1);

    // perform perspective divide
    //vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    vec4 projCoords = FragPosLightSpace * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, FragPosLightSpace.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = FragPosLightSpace.z;
    // check whether current frag pos is in shadow
    float shadow = (currentDepth - 0.1) < closestDepth ? 0.0 : 1.0; 
    //multiply color by shadow factor
    FragColor = FragColor*0.0;

}