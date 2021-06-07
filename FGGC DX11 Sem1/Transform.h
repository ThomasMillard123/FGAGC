#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
class Transform
{
public:

	Transform(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale);
	~Transform();

	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT4X4 GettransfromationMatrix();
	DirectX::XMFLOAT3 GetPos();

private:
	DirectX::XMFLOAT4X4  cObjectWorld, cTransformationMatrix;
	DirectX::XMFLOAT3 cPos, cRotation, cScale;
};

