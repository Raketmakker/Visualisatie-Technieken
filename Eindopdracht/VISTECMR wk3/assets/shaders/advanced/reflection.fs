#version 330

uniform float time;
in vec2 texCoord;

vec3 ray_trace(vec3 ro, vec3 rd)
{
    float a = dot(rd, rd);
    float b = 2.0 * dot(ro, rd);
    float c = dot(ro, ro) - 1.0;
    float d = b*b - 4.0*a*c;
    
    if (d > 0.0) 
    {
        float t = (-b - d) / (2.0 * a);
        vec3 p = ro + t * rd;
        vec3 n = normalize(p);
        vec3 color = 0.2 * vec3(0.2, 0.1, 0.7);
        vec3 ls1 = normalize(vec3(1.0, 1.0, -2.0));
        vec3 rs1 = normalize(2.0 * dot(ls1, n) * n - ls1);
        vec3 ls2 = normalize(vec3(-10.0, 0.4, -2.0));
        vec3 rs2 = normalize(2.0 * dot(ls2, n) * n - ls2);
        vec3 c1 = vec3(1.0, 0.9, 0.8);
        vec3 c2 = vec3(0.8, 0.9, 1.0);
        float ks = 0.5;
        float kd = 0.4;
        float spec = 7.0;
        color += kd * dot(ls1, n) * c1;
        color += ks * pow(max(0.0, dot(rs1, -rd)), spec) * c1;
        color += kd * dot(ls2, n) * c2;
        color += ks * pow(max(0.0, dot(rs2, -rd)), spec) * c2;
        return color;
    }
    else
        return vec3(0.1, 0.1, 0.1);
}

void main()
{
    vec2 uv = texCoord;
    vec3 ro = vec3(0, 0, -4.0);
    vec3 rd = normalize(vec3(uv.xy / 3.0, 1.0));
    gl_FragColor = vec4(ray_trace(ro, rd), 1.0);
}