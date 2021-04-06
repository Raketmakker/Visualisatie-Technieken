#version 330

out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
    vec4 fragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float shadowCalc(vec4 fragPosLightSpace)
{

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5; // mogelijke fout door slecht omschreven voorbeeld in onderzoek.
    
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    
    return (currentDepth > closestDepth) ? 1.0 : 0.0;
}

void main()
{
    
    // start
    vec3 color = texture(diffuseTexture, fs_in.texCoord).rgb;
    vec3 normal = normalize(fs_in.normal);
    vec3 lightColor = vec3(0.0);

    // ambient
    vec3 ambient = (0.15 * color);

    // diffuse
    vec3 lightDirection = normalize(lightPos - fs_in.fragPos);
    
    float diff = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDirection = normalize(viewPos - fs_in.fragPos);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    
    float spec = pow(max(dot(normal, halfwayDirection), 0.0), 64.0);
    vec3 specular = spec * lightColor;
 
    // calculate shadow
    float shadow = shadowCalc(fs_in.fragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0);
}