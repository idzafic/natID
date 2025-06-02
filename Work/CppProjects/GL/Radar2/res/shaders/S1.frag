#version 410
out vec4 fragColor;  //output color 
uniform vec2 leftClick;
uniform vec2 rightClick;
uniform vec2 viewportSize;
uniform float animStep;

vec4 primjer_smoothstepX()
{
    vec2 uv = (2.0*gl_FragCoord.xy - viewportSize.xy)/viewportSize;
   
    float ss = smoothstep(-0.2, 0.2, uv.x);
    return vec4(ss, 0.0, 0.0, 1.0);
}

vec4 primjer_smoothstepY()
{
    vec2 uv = (2.0*gl_FragCoord.xy - viewportSize.xy)/viewportSize;
   
    float ss = smoothstep(-0.2, 0.2, uv.y);
    return vec4(0.0, ss, 0.0, 1.0);
}

vec4 primjer_smoothstep()
{
    vec2 uv = (2.0*gl_FragCoord.xy - viewportSize.xy)/viewportSize;
   
    vec2 ss = smoothstep(-0.2, 0.2, uv);
    return vec4(vec2(ss), 0.0, 1.0);
}

vec4 primjer_smoothstepLine1()
{
    vec2 uv = (2.0*gl_FragCoord.xy - viewportSize.xy)/viewportSize;
   
    float ss = smoothstep(0.1, 0.0, abs(uv.y));
    return vec4(0.0, ss, 0.0, 1.0);
}

vec4 primjer_smoothstepLine2()
{
    vec2 uv = (2.0*gl_FragCoord.xy - viewportSize.xy)/viewportSize;
   
    float ss = smoothstep(0.01, 0.0, abs(abs(uv.y)-0.5));
    return vec4(0.0, ss, 0.0, 1.0);
}

vec4 primjer_smoothstepLine3()
{
    vec2 uv = (2.0*gl_FragCoord.xy - viewportSize.xy)/viewportSize;
       
	float ss = smoothstep(0.01, 0.0, abs(abs(uv.y) - 0.2*smoothstep(0.2, 0.0, abs(uv.x))));

    return vec4(0.0, ss, 0.0, 1.0);
}

vec4 primjer_krug(float offsetFromSideInPixels)
{   
    float maxCircleRadius = 0.5*min(viewportSize.x, viewportSize.y) - offsetFromSideInPixels;
	
	vec2 centerOfViewport = viewportSize.xy/2.0;
	
	vec2 uv = gl_FragCoord.xy;

	float isInside = 1.0;
    if(length(uv - centerOfViewport)>maxCircleRadius)
        isInside = 0.0;

    return vec4(0.0, isInside, 0.0, 1.0);
}

vec4 primjer_kruznica(float thicknessPixels, float offsetFromSideInPixels)
{   
	vec4 krugVeci = primjer_krug(offsetFromSideInPixels);
	vec4 krugManji = primjer_krug(offsetFromSideInPixels + thicknessPixels);

    return vec4(krugVeci.xyz-krugManji.xyz, 1.0);
}



#define enemy1_col vec3(1.0,0.35,0.35)
#define enemy2_col vec3(1.0,1.0,0.35)

float enemy(vec2 uv, vec2 center, float size)
{
    float r = length(uv - center);
    float maxCircleRadius = 0.5*min(viewportSize.x, viewportSize.y);

    vec2 centerOfViewport = viewportSize.xy/2.0;
    vec2 centerToRadarCoord = center - centerOfViewport;
    if(length(centerToRadarCoord)>maxCircleRadius-36.0)
        return 0.0;
   
    return (0.5+0.5*cos(0.06*animStep)) * smoothstep(size+2.0,size-2.0, r);
}

float point(vec2 uv, vec2 center, float size)
{
    float r = length(uv - center);  
    return smoothstep(size+1.0,size-1.0, r);
}

vec4 primjer_Radar1()
{   
	vec2 uv = gl_FragCoord.xy;
    vec3 color = vec3(0.0);

   	color += enemy(uv,leftClick, 10.0) * enemy1_col;
    color += enemy(uv,rightClick, 10.0) * enemy2_col;
	
    return vec4( color, 1.0 );
}


#define circle_color vec3(0.7,0.9,1.0)

float circle(vec2 uv, vec2 center, float radius, float width)
{
    float r = length(uv - center);
    return smoothstep(radius+1.0,radius-1.0, r-width/2.0) - smoothstep(radius+1.0,radius-1.0, r+width/2.0);
}

vec4 primjer_Radar2()
{   
	vec2 uv = gl_FragCoord.xy;
    vec2 center = viewportSize.xy/2.0;
    vec3 color = vec3(0.0);

    color += enemy(uv,leftClick, 10.0) * enemy1_col;
    color += enemy(uv,rightClick, 10.0) * enemy2_col;


    float maxCircleRadius = 0.5*min(viewportSize.x, viewportSize.y);

    color += circle(uv, center, 0.3*maxCircleRadius, 1.0) * circle_color;
    color += circle(uv, center, 0.5*maxCircleRadius, 2.0) * circle_color;
    color += circle(uv, center, 0.7*maxCircleRadius, 3.0) * circle_color;
    color += circle(uv, center, 0.9*maxCircleRadius, 4.0) * circle_color;

    return vec4( color, 1.0 );
}

float distance_to_line(vec2 P, vec2 A, vec2 B)
{//calc distance of point P to line AB
    vec2 g = B - A;
    vec2 h = P - A;
    return length(h - g * clamp(dot(g, h) / dot(g,g), 0.0, 1.0));
}

#define M_PI 3.1415926535897932384626433832795

float radar_line(vec2 uv, vec2 center, float radius, float lineW)
{
    float angle = 0.5 * animStep;
    float angle_rad = angle*M_PI/180.0;
    vec2 h = uv - center;
    float r = length(h); 
    if(r>radius)
        return 0.0;
   
    //distance of fragment uv to line p
    vec2 p = radius*vec2(cos(angle_rad), -sin(angle_rad));
    float l = length(h - p*clamp(dot(h,p)/dot(p,p), 0.0, 1.0) );

    return smoothstep(lineW, 0.5*lineW, l);
}

float radar_trail(vec2 uv, vec2 center, float radius)
{
    float angle = 0.5 * animStep;
    float angle_rad = angle*M_PI/180.0;
    vec2 h = uv - center;
    float r = length(h); 
    if(r>radius)
        return 0.0;

    h = normalize(h);
   	float theta = mod(atan(h.y,h.x)+angle_rad,2*M_PI);
    float gradient = clamp(1.0-theta*0.4,0.0,0.5);

    return gradient;
}

#define radar_color vec3(0.3,0.9,0.6)
#define trail_color vec3(0.3,0.7,0.9)

vec4 primjer_Radar3()
{   
	vec2 uv = gl_FragCoord.xy;
    vec2 center = viewportSize.xy/2.0;
    vec3 color = vec3(0.0);
    float maxCircleRadius = 0.5*min(viewportSize.x, viewportSize.y);

    float radar_trail = radar_trail(uv, center, 0.9*maxCircleRadius);

    color += enemy(uv,leftClick, 10.0) * enemy1_col * radar_trail;
    color += enemy(uv,rightClick, 10.0) * enemy2_col * radar_trail;


    color += circle(uv, center, 0.3*maxCircleRadius, 1.0) * circle_color;
    color += circle(uv, center, 0.5*maxCircleRadius, 2.0) * circle_color;
    color += circle(uv, center, 0.7*maxCircleRadius, 3.0) * circle_color;
    color += circle(uv, center, 0.9*maxCircleRadius, 4.0) * circle_color;
       
    color += max(radar_trail * trail_color, radar_line(uv, center, 0.9*maxCircleRadius, 4.0) * radar_color);

    color += point(uv,center, 7.0) * radar_color;


    return vec4( color, 1.0 );
}


//linija od tacke A do tacke B
float line_segment(vec2 P, vec2 A, vec2 B, float r) 
{
    float d = distance_to_line(P, A, B);
	return smoothstep(r, 0.5*r, d);
}

vec4 primjer_LineSegment()
{   
	vec2 uv = gl_FragCoord.xy;
    vec2 center = viewportSize.xy/2.0;

    vec3 color = line_segment(uv, center, leftClick, 5) * enemy1_col;
    color += line_segment(uv, center, rightClick, 5) * enemy2_col;

    return vec4(color, 1.0 );
}

void main() 
{
    //fragColor = primjer_LineSegment();
    //fragColor = primjer_Radar1();
    //fragColor = primjer_Radar2();
    fragColor = primjer_Radar3();
}
