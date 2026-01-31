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
const float gamma = 2.2;
const float exposure = 1.0;

vec3 toLinear(vec3 sRGB)
{
    return pow(sRGB, vec3(gamma));
}

void main()
{
    vec3 baseColor = toLinear(fragColor.rgb) * toLinear(colDiffuse.rgb);

    float voxelSize = 0.25f;
    vec3 normal = normalize(fragNormal);
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 l = normalize(-lightDir);

    float NdotL = max(dot(normal, l), 0.0);
    vec3 diffuse = lightColor.rgb * NdotL;

    float specCo = 0.0;
    if (NdotL > 0.0)
    {
        vec3 halfwayDir = normalize(l + viewD);
        specCo = pow(max(dot(normal, halfwayDir), 0.0), 128.0);
    }
    vec3 specular = lightColor.rgb * specCo;

    float shadow = 0.0;
    vec3 snappedPos = floor(fragPosition / voxelSize) * voxelSize + (voxelSize * 0.5);
    vec4 fragPosLightSpace = lightVP * vec4(snappedPos, 1.0);

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z <= 1.0 && projCoords.x >= 0.0 && projCoords.x <= 1.0 && projCoords.y >= 0.0 && projCoords.y <= 1.0)
    {
        float curDepth = projCoords.z;
        float bias = max(0.005 * (1.0 - dot(normal, l)), 0.0005);

        float shadowDepth = texture(shadowMap, projCoords.xy).r;
        shadow = (curDepth - bias > shadowDepth) ? 1.0 : 0.0;
    }

    vec3 ambientTerm = ambient.rgb * baseColor;
    vec3 diffuseTerm = (diffuse * baseColor) * (1.0 - shadow);
    vec3 specularTerm = specular * (1.0 - shadow);

    vec3 linearResult = ambientTerm + diffuseTerm + specularTerm;

    linearResult = vec3(1.0) - exp(-linearResult * exposure);

    finalColor = vec4(pow(linearResult, vec3(1.0 / gamma)), fragColor.a);
}