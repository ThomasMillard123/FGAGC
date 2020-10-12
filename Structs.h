#pragma once
#include <d3d11_1.h>

struct ConstantBuffer
{
DirectX::XMMATRIX mWorld;
DirectX::XMMATRIX mView;
DirectX::XMMATRIX mProjection;
};


struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};