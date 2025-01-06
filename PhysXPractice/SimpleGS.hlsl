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
    // 1. �Է� ������ ��������
    float4 centerPos = input[0].position; // Ŭ�� ���� ��ǥ
    //float size = 6; // ��������Ʈ ũ�� (Ŭ�� ���� ����)

    // 2. �簢���� �� ������ ��ġ ��� (Ŭ�� ���� ������)
    float2 offsets[4] = {
        float2(-size, size),  // Top-left
        float2(size, size),   // Top-right
        float2(-size, -size), // Bottom-left
        float2(size, -size)   // Bottom-right
    };

    // �ؽ�ó ��ǥ (UV ����)
    float2 texCoords[4] = {
        float2(0.0, 0.0), // Top-left
        float2(1.0, 0.0), // Top-right
        float2(0.0, 1.0), // Bottom-left
        float2(1.0, 1.0)  // Bottom-right
    };

    // 3. �� �������� ��Ʈ���� �߰�
    for (int i = 0; i < 4; ++i)
    {
        PS_INPUT output;
        output.position = centerPos; // �߽� ��ġ
        output.position.xy += offsets[i]; // Ŭ�� �������� ������ ����
        output.texCoord = texCoords[i];   // �ؽ�ó ��ǥ ����

        stream.Append(output);
    }

    // 4. ��Ʈ�� ����
    stream.RestartStrip();
}