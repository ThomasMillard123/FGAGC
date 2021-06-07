#include "Appearance.h"

Appearance::Appearance(Material material, Geometry geometry):_Geometry(geometry),_Material(material)
{
	_TextureRV = nullptr;
}

Appearance::~Appearance()
{
	CleanUp();
}

void Appearance::Update(float t)
{

}

void Appearance::Draw(ID3D11DeviceContext* pImmediateContext)
{
	
	// Set vertex and index buffers
	pImmediateContext->IASetVertexBuffers(0, 1, &_Geometry.vertexBuffer, &_Geometry.vertexBufferStride, &_Geometry.vertexBufferOffset);
	pImmediateContext->IASetIndexBuffer(_Geometry.indexBuffer, DXGI_FORMAT_R16_UINT,0);

	pImmediateContext->DrawIndexed(_Geometry.numberOfIndices, 0, 0);
}

void Appearance::CleanUp()
{
	if (_TextureRV) _TextureRV->Release();
}
