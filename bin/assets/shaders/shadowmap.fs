#version 330

in vec3 fragPosition;
in vec4 fragColor;
in vec3 fragNormal;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

uniform vec3 lightDir;
uniform vec4 lightColor;
uniform vec4 ambient;
uniform vec3 viewPos;

uniform mat4 lightVP;
uniform sampler2D shadowMap;
uniform int shadowMapResolution;

void main()
{
    vec3 normal = normalize(fragNormal);
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 l = -lightDir;
    float NdotL = max(dot(normal, l), 0.0);
    vec3 diffuse = lightColor.rgb * NdotL;

    float specCo = 0.0;
    if (NdotL > 0.0)
        specCo = pow(max(0.0, dot(viewD, reflect(-l, normal))), 16.0);
    vec3 specular = lightColor.rgb * specCo;


    float shadow = 0.0;
    vec4 fragPosLightSpace = lightVP * vec4(fragPosition, 1.0);
    fragPosLightSpace.xyz /= fragPosLightSpace.w;
    fragPosLightSpace.xyz = (fragPosLightSpace.xyz + 1.0) / 2.0;

    if(fragPosLightSpace.z <= 1.0 && fragPosLightSpace.x >= 0.0 && fragPosLightSpace.x <= 1.0 && fragPosLightSpace.y >= 0.0 && fragPosLightSpace.y <= 1.0)
    {
        float curDepth = fragPosLightSpace.z;
        float bias = max(0.00025 * (1.0 - dot(normal, l)), 0.000025);

        vec2 texelSize = 1.0 / vec2(shadowMapResolution);
        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                float pcfDepth = texture(shadowMap, fragPosLightSpace.xy + vec2(x, y) * texelSize).r;
                shadow += (curDepth - bias > pcfDepth ? 1.0 : 0.0);
            }
        }
        shadow /= 9.0;
    }


    vec3 lighting = (ambient.rgb / 10.0) + (1.0 - shadow) * (diffuse + specular);
    vec3 result = fragColor.rgb * lighting * colDiffuse.rgb;
    finalColor = vec4(result, fragColor.a);
    finalColor.rgb = pow(finalColor.rgb, vec3(1.0/2.2));
}