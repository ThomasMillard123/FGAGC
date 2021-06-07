#pragma once
#include<vector>
#include"ParticalSystem.h"
#include"Observer.h"
using namespace std;

class ParticalSystem_Manager:public Observer
{
public:
	ParticalSystem_Manager();
	~ParticalSystem_Manager();

	void AddParticalSystem(ParticalSystem* newSystem);
	void RemoveParticalSystem(ParticalSystem* newSystem);
	void ClearList();
	void Update(float DeltaTime);
	void Draw(ID3D11DeviceContext* pImmediateContext, ConstantBuffer* pConstantBuffer);


	void OnNotify(const void* entertity, Events event);
private:
	vector<ParticalSystem*> _ParticalSystems;
};

