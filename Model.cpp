#include "Model.h"

Model::Model(ID3D11Buffer* _PConstantBuffer,  ID3D11DeviceContext* _PImmediateContext, ID3D11VertexShader* _PVertexShader, ID3D11PixelShader* _PPixelShader, float PositionX, float PositionY, float PositionZ) {

	
	positionX = PositionX;
	positionY = PositionY;
	positionZ = PositionZ;
	cAngleX = 0;
	cAngleY = 0;
	cAngleZ = 0;

	_pConstantBuffer = _PConstantBuffer;
	_pImmediateContext = _PImmediateContext;
	_pVertexShader=_PVertexShader;
	_pPixelShader=_PPixelShader;


	 

}

void Model::Update(float t,bool rotationX, bool rotationY, bool rotationZ) {
	//
	// Animate the cube
	//
	if (rotationX == true) {
		cAngleX= t;
	}
	 if (rotationY==true)
	{
		cAngleY = t;
	}
	 if (rotationZ == true) {
		cAngleZ = t;
	}
	
	
	
	
}

void Model::Draw(DirectX::XMFLOAT4X4  _view, DirectX::XMFLOAT4X4 _projection ) {

	
	DirectX::XMMATRIX mWorld = XMLoadFloat4x4(&_cWorld);
	DirectX::XMMATRIX view = XMLoadFloat4x4(&_view);
	DirectX::XMMATRIX projection = XMLoadFloat4x4(&_projection);
	//
	// Update variables
	//
	
	ConstantBuffer cb;
	cb.mWorld = DirectX::XMMatrixTranspose(mWorld);
	cb.mView = DirectX::XMMatrixTranspose(view);
	cb.mProjection = DirectX::XMMatrixTranspose(projection);

	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	
	//
	// Renders a triangle
	//
	
	_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);
	_pImmediateContext->DrawIndexed(36, 0, 0);


}


Model::~Model() {


}