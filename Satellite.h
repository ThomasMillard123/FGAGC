#pragma once
#include "Model.h"
class Satellite :
	public Model
{
public:
	Satellite(ID3D11Buffer* _PConstantBuffer, ID3D11DeviceContext* _PImmediateContext, ID3D11VertexShader* _PVertexShader, ID3D11PixelShader* _PPixelShader, float PositionX, float PositionY, float PositionZ);
	~Satellite();

	void Update(float t, bool rotationX, bool rotationY, bool rotationZ);
	void Draw(DirectX::XMFLOAT4X4  _view, DirectX::XMFLOAT4X4 _projection);
};

