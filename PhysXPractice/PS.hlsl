
#include "Header.hlsli"


#define GAMMA 2.2
//PS
float4 PS(PS_INPUT input) : SV_Target
{

    float4 finalColor = { 0.0f, 0.0f, 0.0f, 1.0f };

    float3 normal = normalize(input.normal);
    float3 viewDir = normalize((float3) cameraPosition - (float3) input.worldPosition);
    float3 lightVector = normalize(-light_direction);
    float3 halfVector = normalize(lightVector + viewDir);
    float NdotV = saturate(dot(normal, viewDir));

 
    if (hasNormalMap)
    {
        float3 normalTangentSpace = normalTexture.Sample(linearSampler, input.texCoord).rgb * 2.0 - 1.0;
        float3x3 worldTransform = float3x3(input.tangent, cross(input.normal, input.tangent), input.normal);
        normal = normalize(mul(normalTangentSpace, worldTransform));
    }
    if (hasAmbientMap)
    {
        float4 ambientColor
    = ambientTexture.Sample(linearSampler, input.texCoord);
        finalColor += ambientColor;
    
    }
    else
        finalColor += 0.2f;
    
    if (hasDiffuseMap)
    {
        float4 light = saturate(dot(normalize((float3) -light_direction), normalize(normal)));
        float4 diffuseColor
    = diffuseTexture.Sample(linearSampler, input.texCoord) * light;
        finalColor += diffuseColor;
    }
    else
    {
        float4 light = saturate(dot(normalize((float3) -light_direction), normalize(normal)));
        float4 diffuseColor = light * 0.2f;
        finalColor += diffuseColor;
    }
    
    if (hasSpecularMap)
    {
        float3 viewDir = normalize((float3) cameraPosition - (float3) input.worldPosition);
        float3 halfVector = normalize(-normalize((float3) light_direction) + viewDir);
        float fHDotN = max(0.0f, dot(halfVector, normal));
        float4 specularColor = pow(fHDotN, 320) * specularTexture.Sample(linearSampler, input.texCoord);
        finalColor += specularColor;
    }
    else
    {
        float3 viewDir = normalize((float3) cameraPosition - (float3) input.worldPosition);
        float3 halfVector = normalize(-normalize((float3) light_direction) + viewDir);
        float fHDotN = max(0.0f, dot(halfVector, normal));
        float4 specularColor
    = pow(fHDotN, 320) * 0.2f;
        finalColor += specularColor;
    }
    
    return finalColor;
}

