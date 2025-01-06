struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

cbuffer SizeBuffer : register(b1) 
{
    float size;
    float padding[3];
};

[maxvertexcount(4)]
void main(point PS_INPUT input[1], inout TriangleStream<PS_INPUT> stream)
{
    // 1. 입력 데이터 가져오기
    float4 centerPos = input[0].position; // 클립 공간 좌표
    //float size = 6; // 스프라이트 크기 (클립 공간 단위)

    // 2. 사각형의 네 꼭짓점 위치 계산 (클립 공간 오프셋)
    float2 offsets[4] = {
        float2(-size, size),  // Top-left
        float2(size, size),   // Top-right
        float2(-size, -size), // Bottom-left
        float2(size, -size)   // Bottom-right
    };

    // 텍스처 좌표 (UV 매핑)
    float2 texCoords[4] = {
        float2(0.0, 0.0), // Top-left
        float2(1.0, 0.0), // Top-right
        float2(0.0, 1.0), // Bottom-left
        float2(1.0, 1.0)  // Bottom-right
    };

    // 3. 각 꼭짓점을 스트림에 추가
    for (int i = 0; i < 4; ++i)
    {
        PS_INPUT output;
        output.position = centerPos; // 중심 위치
        output.position.xy += offsets[i]; // 클립 공간에서 오프셋 적용
        output.texCoord = texCoords[i];   // 텍스처 좌표 매핑

        stream.Append(output);
    }

    // 4. 스트립 종료
    stream.RestartStrip();
}