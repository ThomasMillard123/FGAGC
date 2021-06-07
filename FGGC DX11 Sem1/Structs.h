#pragma once
#include <d3d11_1.h>

struct ConstantBuffer
{
DirectX::XMMATRIX WorldViewProjection;
DirectX::XMMATRIX World;
DirectX::XMMATRIX View;
DirectX::XMMATRIX Projection;
float gTime;
DirectX::XMFLOAT3 LightVecW;
DirectX::XMFLOAT4 DiffuseMtrl;
DirectX::XMFLOAT4 DiffuseLight;
DirectX::XMFLOAT4 AmbientMtrl;
DirectX::XMFLOAT4 AmbientLight;
DirectX::XMFLOAT4 SpecularMtrl;
DirectX::XMFLOAT4 SpecularLight;
float SpecularPower;
DirectX::XMFLOAT3 EyePos;
int NRMEnabled;
int SpecularEnabled;

};

struct LightData {
	DirectX::XMFLOAT3 LightVecW;
	DirectX::XMFLOAT4 DiffuseLight;
	DirectX::XMFLOAT4 AmbientLight;
	DirectX::XMFLOAT4 SpecularLight;
};
struct VertexTangent
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexC;
	DirectX::XMFLOAT3 Tangent;
};

struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexC;
	

	bool operator<(const SimpleVertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
	};

};

struct Vertex {
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 TexC;
};

struct MeshData
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	UINT VBStride;
	UINT VBOffset;
	UINT IndexCount;
};





