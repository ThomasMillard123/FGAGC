#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
class ParticleModel
{
public:
	ParticleModel();
	~ParticleModel();


	void SetRotation(DirectX::XMFLOAT3 rotationSpeed);
	void SetMovement(DirectX::XMFLOAT3 movmentSpeed);
	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetScale(DirectX::XMFLOAT3 size);

private:

};

