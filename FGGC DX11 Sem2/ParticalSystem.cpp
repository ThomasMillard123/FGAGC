#include "ParticalSystem.h"
#include"ParticleModel.h"
#include"Appearance.h"
#include"Transform.h"
#include"ObjectBuilder.h"
#include"C++HelperFunctions.h"



ParticalSystem::ParticalSystem(int maxParticals, Vector3 initPos, string name):MaxNumberOfParticals(maxParticals),Positon(initPos),Name(name)
{
	appranance = nullptr;
	
}

ParticalSystem::~ParticalSystem()
{
	CleanUP();
}

bool ParticalSystem::CreatNewPartical(Vector3 initailPos, Vector3 intiVel, float initMass)
{
	
	if (_NumberOfParticals < MaxNumberOfParticals && appranance != nullptr) {
		Transform* transfrom = new Transform();
		transfrom->SetScale(0.2f, 0.2f, 0.2f);
		transfrom->SetPosition({ (float)RandomGen::randomFloat<float>(Positon.x - _Radius ,Positon.x + _Radius),(float)RandomGen::randomFloat<float>(Positon.y,Positon.y + _MaxHight),(float)RandomGen::randomFloat<float>(Positon.z - _Radius,Positon.z + _Radius) });
		ParticleModel* particalmodle = new ParticleModel(transfrom, true, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,0.0f }, 20);
		_Particals.push_back(new GameObject("Partical", appranance, transfrom, particalmodle));
		_NumberOfParticals++;
		return true;
	}
	return false;
}
//remove from list
bool ParticalSystem::DestoryPartical(GameObject* Partical)
{
	
	vector<GameObject*>::iterator iter = _Particals.begin();
	while (iter != _Particals.end()) {
		if (*iter == Partical) {
			iter = _Particals.erase(iter);
			return true;
		}
		else
		{
			++iter;
		}
	}
	return false;
}

bool ParticalSystem::ResetPartical(GameObject* Partical)
{
	Partical->GetTransfrom()->SetScale(0.2f, 0.2f, 0.2f);
	Partical->GetTransfrom()->SetPosition({ (float)RandomGen::randomFloat<float>(Positon.x - _Radius ,Positon.x + _Radius),(float)RandomGen::randomFloat<float>(Positon.y,Positon.y + _MaxHight),(float)RandomGen::randomFloat<float>(Positon.z - _Radius,Positon.z + _Radius) });
	
	Partical->SetLifeSpan(RandomGen::randomFloat<float>(0, (float)MaxlifeSpan/2));

	return true;
}
void ParticalSystem::Draw(ID3D11DeviceContext* pImmediateContext, ConstantBuffer* pConstantBuffer)
{

	ConstantBuffer cb = *pConstantBuffer;

	for (auto partical : _Particals) {
		// Get render material
		Material material = partical->GetAppearance()->GetMaterial();

		// Copy material to shader
		cb.surface.AmbientMtrl = material.ambient;
		cb.surface.DiffuseMtrl = material.diffuse;
		cb.surface.SpecularMtrl = material.specular;

		// Set world matrix
		cb.World = XMMatrixTranspose(XMLoadFloat4x4(&partical->GetTransfrom()->GetWorldMatrix()));

		// Set texture
		if (partical->GetAppearance()->HasTexture())
		{
			ID3D11ShaderResourceView* textureRV = partical->GetAppearance()->GetTextureRV();
			pImmediateContext->PSSetShaderResources(0, 1, &textureRV);
			cb.HasTexture = 1.0f;
		}
		else
		{
			cb.HasTexture = 0.0f;
		}

		// Update constant buffer
		pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

		partical->Draw(pImmediateContext);
	}
}

void ParticalSystem::LoadParticals()
{
	for (UINT i = 0; i < MaxNumberOfParticals; i++)
	{
		CreatNewPartical(Positon, { 0,-10,0 }, 20);
	}

}

void ParticalSystem::AddToInAction(GameObject* Partical)
{
	_ToInAction.push_back(Partical);
}

bool ParticalSystem::RemoveFromInAction(GameObject* Partical)
{
	vector<GameObject*>::iterator iter = _ToInAction.begin();
	while (iter != _ToInAction.end()) {
		if (*iter == Partical) {
			iter = _ToInAction.erase(iter);
			return true;
		}
		else
		{
			++iter;
		}
	}
	return false;
}

void ParticalSystem::CleanUP()
{

	_Particals.clear();

	delete appranance;
	appranance = nullptr;
}
