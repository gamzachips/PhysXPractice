
struct PS_INPUT
{
    float4 position : SV_POSITION;
};

float4 PS(PS_INPUT input) : SV_Target
{
    return float4(0.f, 0.f, 1.f, 1.f);
}