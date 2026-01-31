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
// uniform int shadowMapResolution; // Nieużywany w kodzie, usunąć jeśli zbędny

// Stałe do kalibracji
const float gamma = 2.2;
const float exposure = 1.0; // Zwiększ/zmniejsz, aby sterować ogólną jasnością sceny

vec3 toLinear(vec3 sRGB)
{
    return pow(sRGB, vec3(gamma));
}

void main()
{
    // 1. KOREKTA KOLORU: Konwersja wejścia z sRGB do Linear Space
    // Jeśli tekstura/kolor są w sRGB, musimy je "zlinearyzować" przed obliczeniami światła.
    // Inaczej matematyka oświetlenia jest błędna.
    vec3 baseColor = toLinear(fragColor.rgb) * toLinear(colDiffuse.rgb);
    // Jeśli używasz texture0, odkomentuj i użyj:
    // vec3 texColor = toLinear(texture(texture0, UV).rgb);
    // baseColor *= texColor;

    float voxelSize = 0.25f;
    vec3 normal = normalize(fragNormal);
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 l = normalize(-lightDir); // Znormalizuj wektor światła dla pewności

    float NdotL = max(dot(normal, l), 0.0);
    vec3 diffuse = lightColor.rgb * NdotL;

    float specCo = 0.0;
    if (NdotL > 0.0)
    {
        // Blinn-Phong jest stabilniejszy niż Phong
        vec3 halfwayDir = normalize(l + viewD);
        specCo = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    }
    vec3 specular = lightColor.rgb * specCo;

    // Cienie
    float shadow = 0.0;
    vec3 snappedPos = floor(fragPosition / voxelSize) * voxelSize + (voxelSize * 0.5);
    vec4 fragPosLightSpace = lightVP * vec4(snappedPos, 1.0);

    // Manualna perspektywa
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z <= 1.0 && projCoords.x >= 0.0 && projCoords.x <= 1.0 && projCoords.y >= 0.0 && projCoords.y <= 1.0)
    {
        float curDepth = projCoords.z;
        // Bias zależny od kąta jest poprawny, ale wartości muszą być dobrane do sceny
        float bias = max(0.005 * (1.0 - dot(normal, l)), 0.0005);

        // PCF (Percentage-closer filtering) - opcjonalnie dla miękkich cieni, tu prosty sampling
        float shadowDepth = texture(shadowMap, projCoords.xy).r;
        shadow = (curDepth - bias > shadowDepth) ? 1.0 : 0.0;
    }

    // 2. OBLICZENIE OŚWIETLENIA (W przestrzeni liniowej)
    // Usunięto dzielenie przez 10.0. Ambient powinien być ustawiony poprawnie na CPU.
    // Jeśli ambient jest za jasny, zmniejsz go w uniformie, nie w shaderze.
    vec3 ambientTerm = ambient.rgb * baseColor;
    vec3 diffuseTerm = (diffuse * baseColor) * (1.0 - shadow);
    vec3 specularTerm = specular * (1.0 - shadow); // Specular nie mnożymy przez kolor obiektu (zazwyczaj)

    vec3 linearResult = ambientTerm + diffuseTerm + specularTerm;

    // 3. TONE MAPPING (Reinhard)
    // To rozwiązuje problem "zbyt jaskrawe". Kompresuje wartości HDR [0...N] do LDR [0...1].
    linearResult = vec3(1.0) - exp(-linearResult * exposure);

    // 4. GAMMA CORRECTION
    // Powrót do sRGB dla monitora
    finalColor = vec4(pow(linearResult, vec3(1.0 / gamma)), fragColor.a);
}