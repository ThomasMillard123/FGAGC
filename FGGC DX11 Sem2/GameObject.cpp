#include "GameObject.h"

GameObject::GameObject(string type, Appearance* appearance, Transform* transfrom, ParticleModel* particalModle) : _Appearance(appearance), _Type(type), _Transform(transfrom), _ParticleModel(particalModle)
{

}

GameObject::~GameObject()
{
	CleanUp();
}
static bool upapplied=false;
void GameObject::Update(float t)
{

	if (_ParticleModel != nullptr) {
		_ParticleModel->Update(t);
	}
	_Appearance->Update(t);

}

void GameObject::Draw(ID3D11DeviceContext * pImmediateContext)
{
	

	_Appearance->Draw(pImmediateContext);
}

void GameObject::CleanUp()
{
	delete _ParticleModel;
	_ParticleModel = nullptr;
	delete _Appearance;
	_Appearance = nullptr;
	delete _Transform;
	_Transform = nullptr;
}
