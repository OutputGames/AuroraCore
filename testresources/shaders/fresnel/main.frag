#version 450
#pragma optimize(off)

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec3 camPos;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform PostProcessingSettings {
    float gamma;
    float radiancePower;
} postProcessingSettings;

const int MAX_LIGHTS=4;

layout(binding = 2) uniform LightingSettings {

    vec3 lightPositions[MAX_LIGHTS];
    vec3 lightColors[MAX_LIGHTS];
    vec4 lightPowers;
    int lightAmount;

    vec3 albedo;
    float metallic;
    float roughness;

} lightingSettings;

vec3 gammaCorrect(vec3 v) {


    
    v.rgb = pow(v.rgb, vec3(1.0/postProcessingSettings.gamma));

    return v;
}

/*
vec3 getNormalFromMap()
{
    
    vec3 normal = texture(normalMap, fragTexCoord).xyz;

    //normal.g = 1.0;

    normal = gammaCorrect(normal);

    vec3 tangentNormal = normal * 2.0 - 1.0;



    vec3 Q1  = dFdx(fragPos);
    vec3 Q2  = dFdy(fragPos);
    vec2 st1 = dFdx(fragTexCoord);
    vec2 st2 = dFdy(fragTexCoord);

    vec3 N   = normalize(fragNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
*/

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------

void main() {

    vec3 N = normalize(fragNormal);
    vec3 V = normalize(camPos - fragPos);

    vec3 diffuse = lightingSettings.albedo;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, diffuse, lightingSettings.metallic);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < lightingSettings.lightAmount; ++i) {
        if (lightingSettings.lightColors[i] == vec3(0.0)) {
            continue;
        }

        // calculate per-light radiance
        vec3 L = normalize(lightingSettings.lightPositions[i] - fragPos);
        vec3 H = normalize(V + L);
        float distance = length(lightingSettings.lightPositions[i] - fragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = (lightingSettings.lightColors[i] * attenuation) * lightingSettings.lightPowers[i];

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, lightingSettings.roughness);   
        float G   = GeometrySmith(N, V, L, lightingSettings.roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - lightingSettings.metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * diffuse / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }

    vec3 ambient = vec3(0.03) * diffuse;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  

    outColor = vec4(color,1.0);

    //float fresnel = dot(N, V);

    //fresnel = clamp(1 - fresnel,0.0,1.0);

    //fresnel = pow(fresnel,4);



    //outColor = vec4(vec3(fresnel),1.0);
}