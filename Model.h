#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

#include "resource.h"
#include"Structs.h"
class Model
{
public:

	

	Model(ID3D11Buffer* _PConstantBuffer, ID3D11DeviceContext* _PImmediateContext, ID3D11VertexShader* _PVertexShader, ID3D11PixelShader* _PPixelShader, float PositionX, float PositionY, float PositionZ);
	~Model();

	virtual void Update(float t, bool rotationX, bool rotationY, bool rotationZ);
	virtual void Draw(DirectX::XMFLOAT4X4  _view, DirectX::XMFLOAT4X4 _projection );

protected:
	float positionX, positionY, positionZ;
	float cAngleX, cAngleY, cAngleZ;
	ID3D11DeviceContext* _pImmediateContext;
	ID3D11VertexShader* _pVertexShader;
	ID3D11PixelShader* _pPixelShader;
	ID3D11Buffer* _pConstantBuffer;
	DirectX::XMFLOAT4X4  _cWorld;
	
};

