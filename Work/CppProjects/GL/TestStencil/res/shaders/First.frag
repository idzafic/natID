#version 410

const float gamma = 2.2;

in vec2 fragTexCoord;
uniform sampler2D texWoodFloor;
uniform sampler2D texWoodFloorNorm;

in vec4 v_normal;
in vec3 FragPos;

out vec4 fragColor;

in vec3 LightPos;
uniform vec3 lightColor;


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


//ambient  = material.ambient_color  * light.ambient  * texture_ambient;
//diffuse  = material.diffuse_color  * light.diffuse  * diffuse_strength  * texture_diffuse;
//specular = material.specular_color * light.specular * specular_strength * texture_specular;


void main()
{

    vec4 color1 = texture(texWoodFloor, fragTexCoord);
    color1.rgb = pow(color1.rgb, vec3(gamma));

    fragColor = vec4(phong(color1.xyz), color1.w);

    // apply gamma correction
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}

