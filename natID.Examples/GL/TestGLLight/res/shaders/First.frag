#version 410

in vec2 fragTexCoord;

uniform sampler2D texWoodFloor;
uniform sampler2D texWoodFloorNorm;
in vec4 v_normal;
in vec3 FragPos;

out vec4 fragColor;

in vec3 LightPos;

vec3 ads(vec3 modelColor)
{    
    //modelColor = vec3(0.0, 1.0, 0.0);


    vec3 LightPosition = LightPos;//vec3(0.0, 10.0, 10.0);

    float mKa = 0.1, mKd = 1.0, mKs = 0.4;
    vec3 Ka = vec3(1.0, 1.0, 1.0); // Ambient 
    vec3 Kd = vec3(1.0, 1.0, 1.0); // Diffuse 
    vec3 Ks = vec3(1.0, 1.0, 1.0); // Specular 
    float Shininess = 128;

    vec3 n = normalize( v_normal.xyz );
    vec3 s = normalize( LightPosition - FragPos );
    vec3 v = normalize(vec3(-FragPos));
    vec3 r = reflect( -s, n );

    return
    modelColor * //LightIntensity *
    ( 
    mKa * Ka +
    mKd * Kd * max( dot(s, n), 0.0 ) +
    mKs * Ks * pow( max( dot(r,v), 0.0 ), Shininess )
    );
}


void main()
{
    vec4 color1 = texture(texWoodFloor, fragTexCoord);

    fragColor = vec4(ads(color1.xyz), color1.w);
}

