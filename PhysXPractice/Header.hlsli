#ifndef HEADER_INCLUDED
#define HEADER_INCLUDED

Texture2D ambientTexture : register(t0);
Texture2D diffuseTexture : register(t1);
Texture2D specularTexture : register(t2);
Texture2D normalTexture : register(t3);

SamplerState linearSampler : register(s0);

struct VS_INPUT
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
    float3 tangent : TANGENT;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
    float4 worldPosition : TEXCOORD1;
    float3 tangent : TANGENT;
};

cbuffer TransformBuffer : register(b0) //vs
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
};

cbuffer LightBuffer : register(b1) //ps
{
    float3 light_direction;
    int padding4;
    float4 cameraPosition;
};


cbuffer MaterialProperties : register(b2) //ps
{
    float4 materialColor;
    int hasAmbientMap;
    int hasDiffuseMap;
    int hasSpecularMap;
    int hasNormalMap;
};

#endif