#version 410 core

const float gamma = 2.2;

in vec2 fragTexCoord;
uniform sampler2D texWoodFloor;
uniform sampler2DShadow shadowMap;

in vec4 v_normal;
in vec3 FragPos;

out vec4 fragColor;

in vec3 LightPos;
uniform vec3 lightColor;

in vec4 ShadowCoord;



vec3 phong(vec3 modelColor)
{    
    vec3 LightPosition = LightPos;

    float mKa = 0.1, mKd = 1.0, mKs = 0.4;
    vec3 Ka = lightColor; // Ambient 
    vec3 Kd = lightColor; // Diffuse 
    vec3 Ks = lightColor; // Specular 
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

void main()
{

    vec4 color1 = texture(texWoodFloor, fragTexCoord);
    color1.rgb = pow(color1.rgb, vec3(gamma));
    float bias = 0.001;
    float visibility = texture( shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z - bias)/ShadowCoord.w) );
    visibility = clamp(visibility, 0.25, 1.0);
    //visibility = 1.0;
    
    fragColor = visibility * vec4(phong(color1.rgb), color1.a);

    //fragColor = vec4(1.0,1.0,0,1.0);

    // apply gamma correction
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}

