#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

struct Material
{
	DirectX::XMFLOAT4 cDiffuseMtrl;
	DirectX::XMFLOAT4 cAmbientMaterial;
	DirectX::XMFLOAT4 cSpecularMtrl;
	float cSpecularPower;

};
class Appearance
{
public:
	Appearance();
	~Appearance();

	void SetTextures(ID3D11ShaderResourceView* textureDiffuse, ID3D11ShaderResourceView* textureSpec, ID3D11ShaderResourceView* textureNormMap);
private:
	Material cMaterialType;

	ID3D11ShaderResourceView* cpTextureRV;
	ID3D11ShaderResourceView* cpTextureRVSpec;
	ID3D11ShaderResourceView* cpTextureRVNorm;

	bool cNRMEnable;
	bool cSpecEnable;
};

