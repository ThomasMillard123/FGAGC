#include "Planet.h"

Planet::Planet(ID3D11Buffer* _PConstantBuffer, ID3D11DeviceContext* _PImmediateContext, ID3D11VertexShader* _PVertexShader, ID3D11PixelShader* _PPixelShader, float PositionX, float PositionY, float PositionZ) :Model(_PConstantBuffer, _PImmediateContext, _PVertexShader, _PPixelShader, PositionX, PositionY, PositionZ) {

}
void Planet::Update(float t, bool rotationX, bool rotationY, bool rotationZ) {
Model::Update(t, rotationX, rotationY, rotationZ);
}

void Planet::Draw(DirectX::XMFLOAT4X4  _view, DirectX::XMFLOAT4X4 _projection) {
	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(1, 1, 1);
	DirectX::XMMATRIX mRotationX = DirectX::XMMatrixRotationX(cAngleX);
	DirectX::XMMATRIX mRotationY = DirectX::XMMatrixRotationY(cAngleY);
	DirectX::XMMATRIX mRotationZ = DirectX::XMMatrixRotationZ(cAngleZ);
	DirectX::XMMATRIX mTraslation = DirectX::XMMatrixTranslation(positionX, positionY, positionZ);

	DirectX::XMMATRIX mWorld =
		mScale *
		mRotationX *
		mRotationY *
		mRotationZ *
		mTraslation *
		DirectX::XMMatrixIdentity();

	DirectX::XMStoreFloat4x4(&_cWorld, mWorld);
	Model::Draw(_view, _projection);
}




Planet::~Planet() {

}