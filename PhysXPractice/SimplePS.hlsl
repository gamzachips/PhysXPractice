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

// ���� �� ó�� (��: ������ ������ �ȼ� ����)
    if (color.a < 0.1)
        discard;

    return color;
}