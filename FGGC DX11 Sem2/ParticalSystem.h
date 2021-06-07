#pragma once
#include <directxmath.h>
#include <d3d11_1.h>
#include<vector>

#include"GameObject.h"
#include"Vector3.h"
#include"Structures.h"

using namespace DirectX;
using namespace std;

typedef vector<GameObject*> Particals;

class ParticalSystem
{
public:
	ParticalSystem(int maxParticals,Vector3 initPos,string name);
	~ParticalSystem();

	bool CreatNewPartical(Vector3 initailPos, Vector3 intiVel,float initMass);
	bool DestoryPartical(GameObject* Partical);
	bool ResetPartical(GameObject* Partical);

	virtual void Update(float DeltaTime)=0;
	void Draw(ID3D11DeviceContext* pImmediateContext, ConstantBuffer* pConstantBuffer);


	void Setapprance(Appearance* app) { appranance = app; }
	void SetConstantBuffer(ID3D11Buffer* pConstantBuffer) { _pConstantBuffer = pConstantBuffer; }
	void LoadParticals();

	void AddToInAction(GameObject* Partical);
	bool RemoveFromInAction(GameObject* Partical);


	void SetRadius(int Radius) { _Radius = Radius; }
	void SetMaxHight(int Hight) { _MaxHight = Hight; }
	void SetMaxLifeSpan(int newMaxlifespan) { MaxlifeSpan = newMaxlifespan; }

protected:
	void CleanUP();
protected:
	string Name;

	int MaxlifeSpan;

	int MaxNumberOfParticals;
	int _NumberOfParticals=0;

	//partical gemoatry


	//list of particals
	Particals _Particals;
	Particals _ToInAction;

	Vector3 Positon;
	Appearance* appranance;
	


	int _Radius;
	int _MaxHight;


	ID3D11Buffer* _pConstantBuffer;
};

