#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>

#include"Transform.h"
#include"Appearance.h"
#include"ParticleModel.h"

using namespace DirectX;
using namespace std;
/// <summary>
/// Class to hold all gameobject data
/// </summary>
class GameObject
{
public:
	GameObject(string type, Appearance* appearance, Transform* transfrom,ParticleModel* particalModle);
	~GameObject();

	//gets
	string GetType() const { return _Type; }
	Appearance* GetAppearance() const { return _Appearance; }
	Transform* GetTransfrom() const { return _Transform; }
	ParticleModel* GetParticalModel() const { return _ParticleModel; }

	void Update(float t);
	void Draw(ID3D11DeviceContext * pImmediateContext);
	
	//life span get/set
	void SetLifeSpan(int life) { _LifeSpan = life; }
	int GetLifeSpan() { return _LifeSpan; }
	void AddToLifeSpan(int addlife) { _LifeSpan += addlife; }

private:
	void CleanUp();
private:
	

	string _Type;
	Appearance* _Appearance;
	Transform* _Transform;
	ParticleModel* _ParticleModel;
	int _LifeSpan = 0;
};

