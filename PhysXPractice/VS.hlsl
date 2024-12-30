#include "Header.hlsli"


//VS
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    output.worldPosition = mul(input.position, world);
    output.position = mul(output.worldPosition, view);
    output.position = mul(output.position, projection);
    output.normal = normalize(mul(input.normal, (float3x3) world));
    output.texCoord = input.texCoord;
    output.tangent = normalize(mul(input.tangent, (float3x3) world));
    return output;
}
