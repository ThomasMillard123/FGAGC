#include "GameObject.h"

GameObject::GameObject(MeshData mesh,DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale,MaterialType materalType) {

	cMesh = mesh;
	cScale = scale;
	cPos = position;
	cRotation = rotation;
	cMaterialType = materalType;
	CreatBoundingItems();
	SetMaterialType();
	UpdateTranfromationMatrix(Parent);
}

void GameObject::Update(float t,TransformationType trasformationNumber) {


	//creat world matrix
	UpdateTranfromationMatrix(trasformationNumber);

}
void GameObject::Draw( ID3D11DeviceContext* immediateContext) {
	
	//set textures
	immediateContext->PSSetShaderResources(0, 1, &cpTextureRV);
	if (cSpecEnable) {
		immediateContext->PSSetShaderResources(1, 1, &cpTextureRVSpec);
		
	}
	if (cNRMEnable) {
		immediateContext->PSSetShaderResources(2, 1, &cpTextureRVNorm);
	}
	else
	{
		immediateContext->PSSetShaderResources(1, 1, &cpTextureRVSpec);
		immediateContext->PSSetShaderResources(2, 1, &cpTextureRVNorm);
	}
	
	//setVertexbuffer and indexbuffers
	immediateContext->IASetVertexBuffers(0, 1, &cMesh.VertexBuffer, &cMesh.VBStride, &cMesh.VBOffset);
	immediateContext->IASetIndexBuffer(cMesh.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//
	// Render object
	//
	immediateContext->DrawIndexed(cMesh.IndexCount, 0, 0);
}

DirectX::XMFLOAT4X4 GameObject::GetWorldMatrix()
{
	return cObjectWorld;
}

DirectX::XMFLOAT4X4 GameObject::GettransfromationMatrix()
{
	return cTransformationMatrix;
}

DirectX::XMFLOAT3 GameObject::getPos()
{
	DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&cPos);
	DirectX::XMFLOAT3 Out;
	XMStoreFloat3(&Out, Position);
	
	return Out;
}

DirectX::XMFLOAT3 GameObject::getDirection()
{
	DirectX::XMVECTOR Position { 0.0f,0.0f,1.0f,0.0f };
	DirectX::XMMATRIX mRotationX = DirectX::XMMatrixRotationX(cRotation.x);
	DirectX::XMMATRIX mRotationY = DirectX::XMMatrixRotationY(cRotation.y);
	DirectX::XMMATRIX mRotationZ = DirectX::XMMatrixRotationZ(cRotation.z);
	DirectX::XMMATRIX TrasformationMatrix= mRotationX *
		mRotationY *
		mRotationZ;
	Position= DirectX::XMVector3TransformCoord(Position, TrasformationMatrix);



	DirectX::XMStoreFloat3(&cDirection, DirectX::XMVector3Normalize(Position));

	return cDirection;
}


DirectX::BoundingSphere GameObject::getBoundingSphere()
{
	return cpBoundingSphere;
}



void GameObject::UpdateTranfromationMatrix(TransformationType trasformationType)
{	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(cScale.x, cScale.y, cScale.z);
	DirectX::XMMATRIX mTraslation = DirectX::XMMatrixTranslation(cPos.x, cPos.y, cPos.z);
	DirectX::XMMATRIX mRotationX = DirectX::XMMatrixRotationX(cRotation.x);
	DirectX::XMMATRIX mRotationY = DirectX::XMMatrixRotationY(cRotation.y);
	DirectX::XMMATRIX mRotationZ = DirectX::XMMatrixRotationZ(cRotation.z);
	DirectX::XMMATRIX TrasformationMatrix;
	switch (trasformationType)
	{
	case Parent:
	 TrasformationMatrix =
		mScale *
		mRotationX *
		mRotationY *
		mRotationZ *
		mTraslation *
		DirectX::XMMatrixIdentity();
		break;
	case Child:
	TrasformationMatrix =
			mScale *
			mTraslation *
			mRotationX *
			mRotationY *
			mRotationZ *
			DirectX::XMMatrixIdentity();
		break;
	default:
		TrasformationMatrix= DirectX::XMMatrixIdentity();
		break;
	}
	

	
	DirectX::XMStoreFloat4x4(&cTransformationMatrix, TrasformationMatrix);
}

void GameObject::SetWorld(DirectX::XMFLOAT4X4 world)
{
	cObjectWorld = world;
}

void GameObject::UpdateConstantBuffer(ConstantBuffer& cb, DirectX::XMFLOAT4X4 parent)
{	
	//apply trasformation matrix to world/Parent
	DirectX::XMMATRIX mWorld = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&parent), DirectX::XMLoadFloat4x4(&cTransformationMatrix));

	DirectX::XMStoreFloat4x4(&cObjectWorld, mWorld);
	//
	// Update cb settings
	//
	cb.World = DirectX::XMMatrixTranspose(mWorld);
	cb.NRMEnabled = cNRMEnable;
	cb.SpecularEnabled = cSpecEnable;

	cb.AmbientMtrl=cAmbientMaterial;
	cb.DiffuseMtrl=cDiffuseMtrl;
	cb.SpecularMtrl =cSpecularMtrl;
	cb.SpecularPower = cSpecularPower;
}

void GameObject::SetMaterialType()
{
	
	switch (cMaterialType)
	{
	case SuperShiny:
		cDiffuseMtrl=  { 0.8f,0.5f,0.5f,1.0f };
		cAmbientMaterial=  { 0.8f,0.5f,0.5f,1.0f };
		cSpecularMtrl = { 0.2f,0.2f,0.2f,1.0f };
		cSpecularPower=20;
		break;
	case Shiny:
		cDiffuseMtrl = { 0.8f,0.5f,0.5f,1.0f };
		cAmbientMaterial = { 0.8f,0.5f,0.5f,1.0f };
		cSpecularMtrl = { 0.2f,0.2f,0.2f,1.0f };
		cSpecularPower = 15;
		break;
	case Center:
		cDiffuseMtrl = { 0.6f,0.5f,0.5f,1.0f };
		cAmbientMaterial = { 0.6f,0.5f,0.5f,1.0f };
		cSpecularMtrl = { 0.2f,0.2f,0.2f,1.0f };
		cSpecularPower = 10;
		break;
	case noShine:
		cDiffuseMtrl = { 0.8f,0.5f,0.5f,1.0f };
		cAmbientMaterial = { 0.8f,0.5f,0.5f,1.0f };
		cSpecularMtrl = { 0.2f,0.2f,0.2f,1.0f };
		cSpecularPower = 5;
		break;
	case LittleReflection:
		cDiffuseMtrl = { 0.8f,0.5f,0.5f,1.0f };
		cAmbientMaterial = { 0.8f,0.5f,0.5f,1.0f };
		cSpecularMtrl = { 0.2f,0.2f,0.2f,1.0f };
		cSpecularPower = 1;
		break;
	default:
		cDiffuseMtrl = { 0.8f,0.5f,0.5f,1.0f };
		cAmbientMaterial = { 0.8f,0.5f,0.5f,1.0f };
		cSpecularMtrl = { 0.2f,0.2f,0.2f,1.0f };
		cSpecularPower = 10;
		break;
	}
}

MaterialType GameObject::GetMaterialType()
{
	return cMaterialType;
}

void GameObject::CleanUp()
{
	if (cpTextureRV)cpTextureRV->Release();
	if (cpTextureRVSpec)cpTextureRVSpec->Release();
	if (cpTextureRVNorm)cpTextureRVNorm->Release();
}

void GameObject::CreatBoundingItems()
{
	int maxRadius=1;
	//get max radis

	DirectX::XMStoreFloat3(&cpBoundingSphere.Center,DirectX::XMLoadFloat3(&cPos));
	cpBoundingSphere.Radius = maxRadius;
}

void GameObject::SetRotation(DirectX::XMFLOAT3 rotationSpeed)
{
	cRotation.x += rotationSpeed.x;
	cRotation.y += rotationSpeed.y;
	cRotation.z += rotationSpeed.z;
}

void GameObject::SetMovement(DirectX::XMFLOAT3 MovmentSpeed)
{
	cPos.x  += MovmentSpeed.x;
	cPos.y += MovmentSpeed.y;
	cPos.z += MovmentSpeed.z;
}

void GameObject::SetPosition(DirectX::XMFLOAT3 Poss)
{
	cPos = Poss;
}

void GameObject::SetScale(DirectX::XMFLOAT3 Size)
{
	cScale = Size;
}

/// <summary>
/// set textures for the object and say what texters it has
/// </summary>

void GameObject::SetTextures(ID3D11ShaderResourceView* textureDiffuse, ID3D11ShaderResourceView* textureSpec, ID3D11ShaderResourceView* textureNormMap)
{
	cpTextureRV = textureDiffuse;

	if (textureSpec == NULL) {
		cSpecEnable = false;
		cpTextureRVSpec = nullptr;
	}
	else {
		cSpecEnable = true;
		cpTextureRVSpec = textureSpec;
	}
	if(textureNormMap==NULL){
		cNRMEnable = false;
		cpTextureRVNorm = nullptr;
	}
	else
	{
		cNRMEnable = true;
		cpTextureRVNorm = textureNormMap;
	}
	
	
}


GameObject::~GameObject() {
	CleanUp();
}