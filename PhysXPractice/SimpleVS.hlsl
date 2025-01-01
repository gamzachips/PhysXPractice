
struct VS_INPUT
{
    float4 position : POSITION;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
};

cbuffer TransformBuffer : register(b0) //vs
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
};

PS_INPUT VS(VS_INPUT input)
{

    PS_INPUT output;
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

    return output;
}

