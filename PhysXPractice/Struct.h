#pragma once
#include "pch.h"

struct MaterialData
{
	Vector4 materialColor = { 1.f, 1.f, 1.f ,1.f };
	int hasAmbientMap = 0;
	int hasDiffuseMap = 0;
	int hasSpecularMap = 0;
	int hasNormalMap = 0;
};

struct VertexData
{
	Vector3 position = { 0, 0, 0 };
	Vector3 normal = { 0, 0, 0 };
	Vector2 texture{};
	Vector3 tangent{};
};

struct BasicVertexData
{
	Vector3 position = { 0, 0, 0 };
};

struct WVPData
{
	Matrix world;
	Matrix view;
	Matrix projection;
};

struct LightData
{
	Vector3 direction = { 0.0f, -1.0f, 1.0f };
	int padding3;
	Vector4 cameraPosition;
};

struct CameraBuffer
{
	Vector3 cameraPosition;  // 카메라 위치
	float padding;          // 패딩
};

struct SizeBuffer
{
	float size;
	float padding[3];
};
