Texture2D particleTexture : register(t0);

SamplerState linearSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

float4 PS(PS_INPUT input) : SV_Target
{
    float4 color = particleTexture.Sample(linearSampler, input.texCoord);

// 알파 값 처리 (예: 완전히 투명한 픽셀 제외)
    if (color.a < 0.1)
        discard;

    return color;
}